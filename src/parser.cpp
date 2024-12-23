#include "parser.hpp"
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <map>

std::vector<int> parseFrequencyListToFrequencyVector(std::ifstream& stream) {
  std::unordered_map<std::string, int> wordlist;
  std::string                          word;

  std::vector<int> frequencies;

  std::string line;
  int         lineNumber = 0;

  while (std::getline(stream, line)) {
    lineNumber++;
    std::istringstream iss(line);
    std::string        word, freq;
    if (std::getline(iss, word, ':') && std::getline(iss, freq)) {
      try {
        wordlist[word] = std::stoi(freq);
      } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid frequency format at line " << lineNumber << std::endl;
      }
    }
  }

  frequencies.resize(wordlist.size(), 0);

  int it = 0;
  for (auto& word : wordlist) frequencies[it++] = word.second;

  std::sort(frequencies.begin(), frequencies.end());

  return frequencies;
}
std::vector<int> parseWordListToFrequencyVector(std::ifstream& stream) {

  std::unordered_map<std::string, int> wordlist;
  std::string                          word;

  std::vector<int> frequencies;
  while (stream >> word) {
    wordlist[word]++;
  }

  frequencies.resize(wordlist.size(), 0);

  int it = 0;
  for (auto& word : wordlist) frequencies[it++] = word.second;

  std::sort(frequencies.begin(), frequencies.end());

  return frequencies;
}

std::vector<int> parseFrequencyList(std::ifstream& stream) {

  std::map<std::string, int> wordlist;
  std::vector<int>           occurences;

  std::string word;
  int         uniqueWordCounter = 0;
  while (stream >> word) {
    if (wordlist.count(word) == 0) {
      occurences.push_back(wordlist[word] = uniqueWordCounter++);
    }
    occurences.push_back(wordlist[word]);
  }

  return occurences;
}


std::vector<int> parseWordList(std::ifstream& stream) {

  std::map<std::string, int> wordlist;
  std::vector<int>           occurences;

  std::string word;
  int         uniqueWordCounter = 0;
  while (stream >> word) {
    if (wordlist.count(word) == 0) {
      occurences.push_back(wordlist[word] = uniqueWordCounter++);
    }
    occurences.push_back(wordlist[word]);
  }

  return occurences;
}

std::string getStringFromIndex(int index) {
  std::string name = "";

  int counter = 'z' - 'a';

  while (index != 0) {
    name += 'a' + index % counter;
    index /= counter;
  }

  return name;
}

void parseWriteFrequencyList(const std::vector<int>& data, std::ofstream& stream) {
  for (int i = 0; i < data.size(); i++) {
    stream << getStringFromIndex(i) << ": " << data[i] << std::endl;
  }
}
