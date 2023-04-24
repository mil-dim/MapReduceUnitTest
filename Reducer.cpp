#include "pch.h"

#include "Reducer.h"

namespace fs = std::filesystem;
//reading all teh files from teh temp dir and reduce them to one
Reducer::Reducer(const std::string& temp_dir) : temp_dir_(temp_dir) {}

std::unordered_map<std::string, int> Reducer::reduce() {
    std::unordered_map<std::string, int> word_counts;

    fs::path temp_path(temp_dir_);
    if (!fs::exists(temp_path)) {
        std::cerr << "Temp directory does not exist: " << temp_dir_ << std::endl;
         return word_counts;
    }

    for (const auto& entry : fs::directory_iterator(temp_dir_)) {
        if (!entry.is_regular_file()) {
            continue; 
            //reading only the files, not the subdirs , or links if exists
        }

        std::ifstream infile(entry.path());
        std::string line;

        while (getline(infile, line)) {
       // std::cout << "after the if of the reducer " << entry  << " " << line << " \n";
            //reading a line and looks for the separator : position
            size_t pos = line.find(":");
            if (pos != std::string::npos) {
                //then splitting by that position, first part is teh word, secont - the count
                std::string word = line.substr(0, pos);
                int count = stoi(line.substr(pos + 1));
                //sumarize all values from the all files cumulativly 
                word_counts[word] += count;
            }
        }
        
//std::cout << "after the while the reducer " << entry << " \n";
    }
    //all the files sumarized values
    return word_counts;
}
