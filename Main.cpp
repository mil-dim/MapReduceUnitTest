#include "pch.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <unordered_map>
#include <boost/filesystem.hpp>

#include <boost/range/iterator_range.hpp>
#include <fstream> // add this line
#include "Reducer.h"
#include "SortOutputByFrequency.h"
#include "Workflow.h"
#include "Mapper.h"
#include "FileManager.h"



namespace fs = boost::filesystem;

static char* env_temp = []() { 
    const char* env_var_name = "TEMP";
    char* env_var_value;
    size_t env_var_size;

    errno_t err = _dupenv_s(&env_var_value, &env_var_size, env_var_name);
    if (err != 0 || env_var_value == nullptr) {
        std::cerr << "Failed to read environment variable " << env_var_name << std::endl;
        return static_cast<char*>(nullptr);
    }

    return env_var_value;
}();


int main_test(int argc, char** argv) {
    int exit_code = 0;

    //if there are no 3 additional argumants - fail
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <input_directory> <output_directory> <temp_directory>" << std::endl;
        //if the arguments are not provided, will drop the Satatus file in the Default TEMP dir
        // Filemanager constructor initialize for D://temp
        FileManager fm(env_temp); 
        //Save the exft message in a file ( hardcoded name result.txt in the class FileManager
        fm.SaveResult("FAIL - not enough argiments - exit 1");
        return 1; //exit code 1 for that error
    }
    //getting the  3 dirs from teh 3 arguments
    std::string input_dir(argv[1]);
    std::string output_dir(argv[2]);
    std::string temp_dir(argv[3]);

    FileManager fmtest(input_dir,output_dir,temp_dir);
     exit_code=fmtest.CheckDirs();

     if (exit_code == 0) {

         //this is teh WorkFlow class, managing all
         Workflow wc(input_dir, output_dir, temp_dir);
         // gets teh exit code how teh process of counting finished
         exit_code = wc.CountWords();
     
    //and saves it in teh out_dir result.txt file by teh FM
    FileManager fm(output_dir);
    if (exit_code == 0) 
        fm.SaveResult("Success");
    else
        fm.SaveResult("FAIL "+std::to_string(exit_code));
}
    //also sends teh exit code to teh shell
    return exit_code;
}
