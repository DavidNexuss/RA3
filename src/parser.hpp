#include <vector>
#include <fstream>

std::vector<int> parseFrequencyListToFrequencyVector(std::ifstream& stream);
std::vector<int> parseWordListToFrequencyVector(std::ifstream& stream);

std::vector<int> parseFrequencyList(std::ifstream& stream);
std::vector<int> parseWordList(std::ifstream& stream);

void parseWriteFrequencyList(const std::vector<int>& data, std::ofstream& stream);
