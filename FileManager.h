#ifndef OUTPUTSAVER_H
#define OUTPUTSAVER_H



#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

class FileManager {
public:
    FileManager(const std::string& output_dir);
    FileManager(const std::string& input_dir, const std::string& output_dir, const std::string& temp_dir);

    int SaveOutput(const std::string& output);
    void SaveResult(const std::string& result);
    int  SaveTemp(const std::string& result, const std::string& filename);
    int CheckDirs();

private:
    std::string output_dir_;
    std::string input_dir_;
    std::string temp_dir_;
    //using this var to persist during the program run and indicate the first use,
    //to clean the temp dir
    static bool is_the_first_instance_;


};

#endif // OUTPUTSAVER_H
