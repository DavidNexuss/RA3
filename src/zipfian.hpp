#include <random>
#include <vector>

class ZipfianGenerator {
  public:
  ZipfianGenerator(double alpha, int n);

  int              generate();
  std::vector<int> generate_sequence(int size);

  private:
  double                                 alpha;                             // Skewness parameter
  int                                    n;                                 // Maximum rank
  double                                 harmonic_sum;                      // Precomputed harmonic sum
  std::vector<double>                    cumulative_probabilities;          // Precomputed cumulative probabilities
  std::mt19937                           generator{std::random_device{}()}; // Random number generator
  std::uniform_real_distribution<double> distribution;                      // Uniform distribution [0, 1)
};

void testZipfian(const std::vector<int>& sequence, double alpha);
