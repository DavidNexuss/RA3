#include "zipfian.hpp"
#include <iostream>
#include "util.hpp"
#include "cardinality.hpp"
#include <fstream>
#include "parser.hpp"

void print_usage(const char* program_name) {
  std::cerr << "Usage:\n";
  std::cerr << "  " << program_name << " (Loader) (Tester)\n";
  std::cerr << "Loader:\n";
  std::cerr << "  --generate <N> <alpha> <n>  Generate a Zipfian sequence of size N with alpha and range n.\n";
  std::cerr << "  --loaddat filename.dat      Loads a dataset from disk.\n";
  std::cerr << "  --loadtxt filename.txt      Loads a dataset from disk.\n";
  std::cerr << "  --help                      Show this help message.\n";
  std::cerr << "Tester:\n";
  std::cerr << "  --test                      Test zipfian property.\n";
  std::cerr << "  --dump                      Writes values and frequency.\n";
  std::cerr << "  --estimate <I>              Estimate cardinality with I number of iterations.\n";
}

int    argc;
char** argv;

bool isArgument(int argix, const std::string& name) {
  return argc > argix && std::string(argv[argix]) == name;
}

int readvalues(int& argix, std::vector<int>& sequence, double alpha) {
  if (isArgument(argix, "--generate")) {

    if (argc < 5) {
      std::cerr << "Error: --generate requires exactly 3 arguments: <N> <alpha> <n> (--test) \n";
      print_usage(argv[0]);
      return 1;
    }

    // Parse the arguments
    int N = std::stoi(argv[2]);
    alpha = std::stod(argv[3]);
    int n = std::stoi(argv[4]);

    if (N <= 0 || alpha <= 0 || n <= 0) {
      std::cerr << "Error: All arguments must be positive numbers.\n";
      exit(1);
    }

    ZipfianGenerator generator(alpha, n);
    sequence = generator.generate_sequence(N);
    argix += 4;

  } else if (isArgument(argix, "--loadtxt")) {
    if (argc < 2) {
      std::cerr << "Error: --loadtxt requires an additional parameter\n";
      print_usage(argv[0]);
      return 1;
    }

    std::string   fileName = argv[2];
    std::ifstream file;
    file.open(fileName);

    if (file.is_open()) {
      sequence = parseWordList(file);
      argix += 2;
    } else {
      std::cerr << "Error opening file!: " << fileName << "\n";
      return 1;
    }
  }

  return 0;
}

int test() {
  int              argix = 1;
  double           alpha = 1.0;
  std::vector<int> sequence;
  if (readvalues(argix, sequence, alpha)) return 1;

  if (isArgument(argix, "--test")) {
    argix++;
    testZipfian(sequence, alpha);
  }

  if (isArgument(argix, "--writetxt")) {
    argix++;
    for (int i = 0; i < sequence.size(); i++) {
      std::cout << sequence[i] << std::endl;
    }
  }

  if (isArgument(argix, "--dump")) {
    argix++;
    std::cout << "Values: ";
    printVector(sequence);
    std::cout << "Frequencies: ";
    printVector(packStreamToFrequency(sequence));
  }

  if (isArgument(argix, "--estimate")) {
    argix++;

    if (argc < argix) {
      std::cerr << "Missing iteration count see usage\n";
      print_usage(argv[0]);
    }
    int iterations = std::stoi(argv[argix]);
    estimateCardinalityReport(sequence, iterations);
  }

  return 0;
}

int main(int argc, char** argv) {
  ::argc = argc;
  ::argv = argv;

  if (argc < 2) {
    print_usage(argv[0]);
    return 1;
  }

  std::string option = argv[1];
  test();

  return 0;
}
/*
int thing() {
  int argix;
  if (isArgument(argix, "--generate")) {
    if (argc < 5) { // Expect 3 additional arguments: N, alpha, n
      std::cerr << "Error: --generate requires exactly 3 arguments: <N> <alpha> <n> (--test) \n";
      print_usage(argv[0]);
      return 1;
    }

    // Parse the arguments
    int    N     = std::stoi(argv[2]); // Size of the sequence
    double alpha = std::stod(argv[3]); // Zipfian alpha parameter
    int    n     = std::stoi(argv[4]); // Range of distinct values

    if (N <= 0 || alpha <= 0 || n <= 0) {
      std::cerr << "Error: All arguments must be positive numbers.\n";
      exit(1);
    }

    ZipfianGenerator generator(alpha, n);
    auto             sequence = generator.generate_sequence(N);
    argix                     = 5;

    if (isArgument(argix, "--test")) {
      argix++;
      testZipfian(sequence, alpha);
    }

    if (isArgument(argix, "--dump")) {
      argix++;
      std::cout << "Values: ";
      printVector(sequence);
      std::cout << "Frequencies: ";
      printVector(packStreamToFrequency(sequence));
    }

    if (isArgument(argix, "--estimate")) {
      argix++;

      if (argc < argix) {
        std::cerr << "Missing iteration count see usage\n";
        print_usage(argv[0]);
      }
      int iterations = std::stoi(argv[argix]);
      experimentPrint(estimateCardinalityReport(sequence, iterations));
    }
  }

  if (option == "--generate") {
  } else if (option == "--help") {
    print_usage(argv[0]);
  } else {
    std::cerr << "Error: Unknown option '" << option << "'.\n";
    print_usage(argv[0]);
    return 1;
  }
} */
