#include "pch.h"

#include "Workflow.h"


namespace fs = boost::filesystem;

Workflow::Workflow(const std::string& input_dir, const std::string& output_dir , const std::string& temp_dir)
    : input_dir_(input_dir), output_dir_(output_dir) , temp_dir_(temp_dir) {}


int Workflow::CountWords() {
    int exit_code = 0;
    Mapper mapper;
    Reducer reducer(temp_dir_);
    std::unordered_map<std::string, int> word_counts;


    //reading the input files, should move this into teh FioleManager some day
    fs::path input_path(input_dir_);
    if (!fs::exists(input_path)) {
        std::cerr << "Input directory does not exist: " << input_dir_ << std::endl;
        return 5;
    }

    //for all the regular  files in teh input dir, run the mapper , and save teh Temp files into the 
    //temp dir with teh same names as input files
    for (auto& entry : boost::make_iterator_range(fs::directory_iterator(input_dir_), {})) {
        if (fs::is_regular_file(entry)) {
            word_counts.clear();
            exit_code = mapper.Map(entry.path().string(), word_counts);
            //std::cout << "another file " <<  entry.path().filename().string()  << " " << word_counts.size() << " \n";
            FileManager fm(output_dir_, input_dir_ ,temp_dir_);
            
            std::string output;
            //composing the output for the temp file, splitting teh map entry 
            //into 2 parts and then stream it into teh file
            for (auto& kv : word_counts) {
                output += kv.first + ": " + std::to_string(kv.second) + "\n";
            }
            //save the output into temp dir with teh same filename as teh file that ws processed 
            exit_code=fm.SaveTemp(output, entry.path().filename().string() );
            //exit code how teh Temp file was saved
        }
    }
   // 

    word_counts.clear();

    word_counts = reducer.reduce();
    //made it better with lambda function in "SaveOutput"
    //SortOutputByFrequency::SortOutput(word_counts);
    std::string output;
    for (auto& kv : word_counts) {
        output += kv.first + ": " + std::to_string(kv.second) + "\n";

    }
    FileManager fm(output_dir_);
     exit_code = fm.SaveOutput(output);
     // Here the TEMPFILE dir should be cleaned, but we want it to survive for now for debuging purposes
    return exit_code;
}
