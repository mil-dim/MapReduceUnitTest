#include "pch.h"

#include "../CSE_687_Project_1/Mapper.h"


int  Mapper::Map(const std::string& filepath, std::unordered_map<std::string, int>& word_counts) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error opening file " << filepath << std::endl;
        return 6;
    }
    std::string word;

    while (file >> word) {
        //various cleaning steps and increment the value of teh map entry
        CleanWord(word );
            //, word_counts);
        ++word_counts[word];
    }
    file.close();
    return 0;
}

//cleaning the word is crazy , for now only removing the punctuation and lower case
//commented code below also splits the words as super-hero on super and hero, but
//need more work, it fails when puncuation is at teh begining, or more than 1 
void Mapper::CleanWord(std::string& word) {
    // Remove punctuation from the word   
    //super-hero become superhero -nice   will becone nice , end. will become end
    if (std::all_of(word.begin(), word.end(), [](char c) {
        return c >= 0 && c <= 127;
        })) {
        word.erase(std::remove_if(word.begin(), word.end(), [](char c) {
            return std::ispunct(c);
            }), word.end());
    }

    // Convert the word to lowercase  lambda function []
    std::transform(word.begin(), word.end(), word.begin(), [](char c) {
        return std::tolower(c);
        });
}
/*   
void Mapper::CleanWord(std::string& word, std::unordered_map<std::string, int>& word_counts) {
    // Find the position of the first punctuation mark in the word
    size_t pos = word.find_first_of(".,:;?!\"'`“”‘’()[]{}<>|/-");

    if (pos != std::string::npos) {
        // If the punctuation is not at the beginning of the word, strip everything after it
        if (pos > 0) {
            std::string stripped = word.substr(pos + 1); // Extract the stripped part
            word = word.substr(0, pos); // Strip the word
            // If the stripped part is longer than 2 characters, count it as a separate word
            if (stripped.size() > 2) {
                ++word_counts[stripped];
            }
        }
        // If the punctuation is at the beginning of the word, remove it and strip everything after the next punctuation
        else {
            size_t next_pos = word.find_first_of(".,:;?!\"'`“”‘’()[]{}<>|/-", pos + 1);
            if (next_pos != std::string::npos) {
                std::string stripped = word.substr(next_pos + 1); // Extract the stripped part
                word = word.substr(0, pos) + word.substr(next_pos); // Strip the word
                // If the stripped part is longer than 2 characters, count it as a separate word
                if (stripped.size() > 2) {
                    ++word_counts[stripped];
                }
            }
            // If there is no punctuation after the initial punctuation, remove it
            else {


                word.erase(pos);
            }
        }
    }

    // Convert the word to lowercase
    std::transform(word.begin(), word.end(), word.begin(), [](char c) {
        return std::tolower(c);
        });
}
*/