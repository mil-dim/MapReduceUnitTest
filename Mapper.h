#ifndef MAPPER_H
#define MAPPER_H

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <algorithm>

class Mapper {
public:
    int Map(const std::string& filepath, std::unordered_map<std::string, int>& word_counts);
    void CleanWordWrapper(std::string& word) {
        CleanWord(word);
    }
private:
    // Helper function to remove punctuation and convert to lowercase
    void CleanWord(std::string& word); // , std::unordered_map<std::string, int>& word_counts);
};

#endif // MAPPER_H
