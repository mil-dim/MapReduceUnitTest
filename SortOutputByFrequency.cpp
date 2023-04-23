#include "pch.h"

#include "SortOutputByFrequency.h"

void SortOutputByFrequency::SortOutput(std::unordered_map<std::string, int>& word_counts) {
    std::vector<std::pair<std::string, int>> word_count_pairs(word_counts.begin(), word_counts.end());
    std::sort(word_count_pairs.begin(), word_count_pairs.end(),
        [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
            return a.second > b.second;
        });

    word_counts.clear();
    for (const auto& pair : word_count_pairs) {
        word_counts.emplace(pair.first, pair.second);
    }
}
