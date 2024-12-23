#include <cstdint>
#include <cmath>
#include "cardinality.hpp"
#include <algorithm>
#include "util.hpp"
#include <unordered_set>
#include <set>

Result estimateCardinalityHyperLogLog(const std::vector<int>& values, int buckets) {
  Timer timer;
  timer.tic();
  Result           result;
  int              p = std::log2(buckets);
  std::vector<int> bucketMax(buckets, 0);

  for (int value : values) {
    uint32_t h        = hashFunction(value);
    int      bucket   = h >> (32 - p);
    int      rank     = countLeadingZeros(h << p) + 1;
    bucketMax[bucket] = std::max(bucketMax[bucket], rank);
  }

  //This formula gives the different values of alpha for the different ranges of bucket sizes
  double alphaMM = 0.7213 / (1 + 1.079 / buckets) * buckets * buckets;
  double sum     = 0.0;

  for (int rank : bucketMax) {
    sum += 1.0 / (1 << rank);
  }
  double rawEstimate = alphaMM / sum;

  if (rawEstimate <= 2.5 * buckets) {
    int zeroBuckets = std::count(bucketMax.begin(), bucketMax.end(), 0);
    if (zeroBuckets > 0) {
      rawEstimate = buckets * std::log(static_cast<double>(buckets) / zeroBuckets);
    }
  } else if (rawEstimate > (1 << 30)) {
    rawEstimate = -std::pow(2, 32) * std::log(1 - rawEstimate / std::pow(2, 32));
  }

  result.error       = 1.04 / sqrt(buckets);
  result.cardinality = rawEstimate;
  result.spentTime   = timer.toc();
  result.memoryUsed  = buckets;

  return result;
}

uint32_t kth_largest(const std::set<uint32_t>& s, int k) {
  auto it = s.rbegin();
  for (int i = 1; i < k; ++i) {
    ++it;
  }
  return *it;
}

// I tried the oroginal method as described in the paper (taking into account rk and sk) but I could not make it work.
Result estimateCardinalityRecord(const std::vector<int>& values, int k) {
  Result result;
  Timer  timer;

  std::set<uint32_t> kset;

  timer.tic();
  int i = 0;
  int j = 0;

  // Fill set with first k distinct elements (hash values) of the stream
  while (j < k && i < values.size()) {
    uint32_t value = hashFunction(values[i++]);
    if (kset.insert(value).second) j++;
  }

  int r = k;
  int s = 0;

  for (; i < values.size(); i++) {
    uint32_t hash       = hashFunction(values[i]);
    auto     minElement = kset.begin();

    //If hash is larger than the kth larges element in the add
    if (hash > kth_largest(kset, k)) {
      r++;
      kset.insert(hash);
      //If hash is larger than the min element substitute
    } else if (hash > *minElement) {
      s++;
      kset.erase(minElement);
      kset.insert(hash);
    }
  }

  double n = values.size();
  double e = std::exp(1);

  double z  = k * pow(1.0 + (1.0 / (double)k), r - k + 1) - 1;
  double se = sqrt(pow(n / (k * e), 1 / (double)k) - 1);

  result.spentTime   = timer.toc();
  result.cardinality = z;
  result.memoryUsed  = k;
  result.error       = se;

  return result;
}

// Instead I used the alternative method by the same authors which only takes into account the parameter rk, I have included the slides of the talk which describe the algorithm
Result estimateCardinalityRecord2(const std::vector<int>& values, int k) {
  Result result;
  Timer  timer;

  std::set<uint32_t> kset;

  timer.tic();

  int j = 0;
  int i = 0;

  // Fill set with first k distinct elements (hash values) of the stream
  while (j < k && i < values.size()) {
    uint32_t value = hashFunction(values[i++]);
    if (kset.insert(value).second) j++;
  }


  int r = k;

  //For each element we find in the stream compute hash value, if hash value is
  //greater than the minimum value and the new value is not in the set
  //remove minimum value and replace it with the new one and increment the r counter
  for (; i < values.size(); i++) {
    uint32_t hash       = hashFunction(values[i]);
    auto     minElement = kset.begin();

    if (hash > *minElement && kset.count(hash) == 0) {
      r++;
      kset.erase(minElement);
      kset.insert(hash);
    }
  }

  double e = std::exp(1);

  double z  = k * pow(1.0 + (1.0 / (double)k), r - k + 1) - 1;
  double se = sqrt(pow(z / (k * e), 1 / (double)k) - 1);

  result.spentTime   = timer.toc();
  result.cardinality = z;
  result.memoryUsed  = k;
  result.error       = se;

  return result;
}

Result estimateCardinality(const std::vector<int>& values, int method, int buckets) {
  if (method == 0) {
    return estimateCardinalityHyperLogLog(values, buckets);
  } else if (method == 1) {
    return estimateCardinalityRecord2(values, buckets);
  } else {
    return estimateCardinalityRecord(values, buckets);
  }
}

Result estimateCardinalityNaive(const std::vector<int>& values) {
  Result result;
  Timer  timer;

  result.memoryUsed = currentHeapSize();
  std::unordered_set<int> uniqueValues;

  timer.tic();
  for (int i = 0; i < values.size(); i++) uniqueValues.insert(values[i]);

  result.memoryUsed  = (currentHeapSize() - result.memoryUsed) / sizeof(int);
  result.cardinality = uniqueValues.size();
  result.spentTime   = timer.toc();

  return result;
}

void estimateCardinalityReport(const std::vector<int>& values, int iterations) {
  std::cerr << "Performing " << iterations << " iterations...\n";

  double cardinality = estimateCardinalityNaive(values).cardinality;
  std::cerr << "Distinct words :" << cardinality << std::endl;

  std::cout << "Buckets, Cardinality, PredictedError, RealError, Time,";
  std::cout << "CardinalityREC, PredictedErrorREC, RealErrorREC, TimeREC" << std::endl;
  for (int k = 4; k < 4096; k *= 2) {
    Result result[2];
    Timer  t;
    for (int i = 0; i < iterations; i++) {
      seedFunction(i);
      result[0] += estimateCardinality(values, 0, k);
      result[1] += estimateCardinality(values, 1, k);
    }

    result[0] /= iterations;
    result[1] /= iterations;

    double realError[2] = {
      (cardinality - result[0].cardinality) / cardinality, (cardinality - result[1].cardinality) / cardinality};

    std::cout << k << ", " << result[0].cardinality << ", " << result[0].error << ", " << realError[0] << ", " << result[0].spentTime << ",";
    std::cout << result[1].cardinality << ", " << result[1].error << ", " << realError[1] << ", " << result[1].spentTime << std::endl;
  }
}
