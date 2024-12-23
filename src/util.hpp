#include <vector>
#include <cstdint>
#include "xxhash64.h"
#include <random>
#include <algorithm>
#include <chrono>
#include "malloc.h"
#include <iostream>
#include "murmur.h"

//=====================[UTILITY FUNCTIONS]==============================================================================

// Unpacks frequency vector to a stream of values, randomizes the values orders too
inline std::vector<int> unpackFrequencyToStream(const std::vector<int>& frequency) {
  std::vector<int> values;

  for (int i = 0; i < frequency.size(); i++) {
    for (int k = 0; k < frequency[i]; k++) values.push_back(i);
  }

  std::shuffle(values.begin(), values.end(), std::default_random_engine());

  return values;
}


inline std::vector<int> packStreamToFrequency(const std::vector<int>& values) {
  std::vector<int> frequencies;
  for (int i = 0; i < values.size(); i++) {
    int val = values[i];
    if (val >= frequencies.size()) frequencies.resize(val + 1);
    frequencies[val]++;
  }
  return frequencies;
}

// Count leading zeros
inline int countLeadingZeros(uint32_t value) {
  if (value == 0) return 32;
  return __builtin_clz(value);
}

//=====================[HASH FUNCTIONS TO BE USED]======================================================================
//
// regular hash
// nohash
// xhash


inline uint32_t seed = 0;

// Hash function to distribute the values
inline uint32_t regularHash(int value) {
  value = ((value >> 16) ^ value) * 0x45d9f3b;
  value = ((value >> 16) ^ value) * 0x45d9f3b;
  value = (value >> 16) ^ value;
  return value;
}

inline uint32_t nohash(int value) { return value; }

inline uint32_t xhash(int value) { return XXHash64::hash(&value, sizeof(value), 0); }
inline uint32_t murmur(int value) {
  uint32_t out;
  MurmurHash3_x86_32(&value, sizeof(value), seed, (void*)&out);
  return out;
}

constexpr auto hashFunction = xhash;

inline void seedFunction(int value) {
  srand(value);
  seed = 0;
}

//========================[MEASURE REAL MEMORY CONSUMPTION FOR STD DATA STRUCTURES]======================================
template <typename T>
struct TrackingAllocator {
  static size_t totalAllocated;

  T* allocate(size_t n) {
    totalAllocated += n * sizeof(T);
    return static_cast<T*>(std::malloc(n * sizeof(T)));
  }

  void deallocate(T* p, size_t n) {
    totalAllocated -= n * sizeof(T);
    std::free(p);
  }
};

template <typename T>
size_t TrackingAllocator<T>::totalAllocated = 0;


inline uint64_t currentHeapSize() {
  return mallinfo().uordblks;
}


//======================[TIMER FOR MEASURING TIME]=====================================================================
class Timer {
  std::chrono::high_resolution_clock::time_point startTime;

  public:
  inline void tic() {
    startTime = std::chrono::high_resolution_clock::now();
  }

  inline double toc() const {
    auto                          endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;
    return elapsed.count() * 1000;
  }
};

inline void printVector(const std::vector<int>& vector) {
  std::cout << "[ ";
  for (int i = 0; i < vector.size(); i++) {
    std::cout << vector[i];
    if (i < vector.size() - 1) std::cout << ", ";
  }

  std::cout << " ]" << std::endl;
}


//===================[LINE FITTING ALGORITHM]=============================================================================
//
inline void fitLine(const std::vector<double>& x, const std::vector<double>& y, double& m, double& b, double& rmse) {
  size_t n = x.size();
  if (n != y.size() || n == 0) {
    throw std::invalid_argument("Input vectors must have the same non-zero size.");
  }

  // Calculate sums
  double Sx = 0, Sy = 0, Sxx = 0, Sxy = 0;
  for (size_t i = 0; i < n; ++i) {
    Sx += x[i];
    Sy += y[i];
    Sxx += x[i] * x[i];
    Sxy += x[i] * y[i];
  }

  // Calculate slope (m) and intercept (b)
  m = (n * Sxy - Sx * Sy) / (n * Sxx - Sx * Sx);
  b = (Sy - m * Sx) / n;

  // Calculate RMSE
  double errorSum = 0.0;
  for (size_t i = 0; i < n; ++i) {
    double predictedY = m * x[i] + b;
    errorSum += (y[i] - predictedY) * (y[i] - predictedY);
  }
  rmse = std::sqrt(errorSum / n);
}
