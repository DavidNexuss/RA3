#include <vector>

struct Result {
  const char* name;
  int         memoryUsed;
  double      spentTime;
  double      cardinality;
  double      error;
  int         buckets;

  Result() {
    memoryUsed  = 0;
    spentTime   = 0;
    cardinality = 0;
    error       = 0;
    name        = 0;
  }
  Result operator+=(const Result& other) {
    memoryUsed += other.memoryUsed;
    spentTime += other.spentTime;
    cardinality += other.cardinality;
    error += other.error;
    return *this;
  }
  Result operator/=(double factor) {
    memoryUsed /= factor;
    spentTime /= factor;
    cardinality /= factor;
    error /= factor;
    return *this;
  }
};

Result estimateCardinalityHyperLogLog(const std::vector<int>& values, int buckets);
Result estimateCardinalityNaive(const std::vector<int>& values);
Result estimateCardinality(const std::vector<int>& values, int method, int buckets);
void   estimateCardinalityReport(const std::vector<int>& values, int iterations);
