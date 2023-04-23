#ifndef SORTOUTPUTBYFREQUENCY_H
#define SORTOUTPUTBYFREQUENCY_H

#include <unordered_map>
#include <string>
#include <algorithm>


class SortOutputByFrequency {
public:
    static void SortOutput(std::unordered_map<std::string, int>& word_counts);
};

#endif // SORTOUTPUTBYFREQUENCY_H
