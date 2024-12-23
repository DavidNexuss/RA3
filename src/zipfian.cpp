#include "zipfian.hpp"
#include <vector>
#include <cmath>
#include <random>
#include <map>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "util.hpp"

ZipfianGenerator::ZipfianGenerator(double alpha, int n) :
  alpha(alpha), n(n), distribution(0.0, 1.0) {

  harmonic_sum = 0.0;
  for (int k = 1; k <= n; ++k) {
    harmonic_sum += 1.0 / std::pow(k, alpha);
  }

  cumulative_probabilities.resize(n + 1, 0.0);
  for (int k = 1; k <= n; ++k) {
    cumulative_probabilities[k] = cumulative_probabilities[k - 1] + (1.0 / std::pow(k, alpha)) / harmonic_sum;
  }
}

int ZipfianGenerator::generate() {
  double u   = distribution(generator);
  int    low = 1, high = n, mid;
  while (low < high) {
    mid = low + (high - low) / 2;
    if (cumulative_probabilities[mid] < u) {
      low = mid + 1;
    } else {
      high = mid;
    }
  }
  return low;
}

std::vector<int> ZipfianGenerator::generate_sequence(int size) {
  std::vector<int> sequence;
  sequence.reserve(size);
  for (int i = 0; i < size; ++i) {
    sequence.push_back(generate() - 1);
  }
  return sequence;
}

void testZipfian(const std::vector<int>& sequence, double alpha) {
  std::map<int, int> frequency_map;

  for (int value : sequence) {
    frequency_map[value]++;
  }

  std::vector<std::pair<int, int>> frequency_vector(frequency_map.begin(), frequency_map.end());
  std::sort(frequency_vector.begin(), frequency_vector.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });

  std::cout << "Rank\tValue\tFrequency\tLog(Rank)\tLog(Frequency)\n";
  int rank = 1;

  std::vector<double> logRank;
  std::vector<double> logFreq;

  for (const auto& pair : frequency_vector) {
    double log_rank = std::log10(rank);
    double log_freq = std::log10(pair.second);
    std::cout << rank << "\t" << pair.first << "\t" << pair.second
              << "\t\t" << std::fixed << std::setprecision(3) << log_rank
              << "\t\t" << log_freq << "\n";
    logRank.push_back(log_rank);
    logFreq.push_back(log_freq);
    rank++;
  }

  double m;
  double b;
  double rmse;

  fitLine(logRank, logFreq, m, b, rmse);

  std::cerr << "Zipf property fiting line: y = mx + b -> " << std::endl;
  std::cerr << "y = " << m << " * x + " << b << "; rmse = " << rmse << std::endl;
}
