#include "pch.h"

#include "FileManager.h"
static char* env_temp;
bool FileManager::is_the_first_instance_ = true;
//two constructors, one taking an output directory, and another taking input, 
//output, and temporary directories.Both constructors take in strings that 
//represent directory paths.These paths are used later by the class functions to readand write files.
FileManager::FileManager(const std::string& output_dir)
    : output_dir_(output_dir) {}
//the second constructor is used for managing TEMP dir and its cleaning it on teh first use
//after it runs once , first time, it clean all teh temp files from teh previous run
FileManager::FileManager(const std::string& input_dir, const std::string& output_dir, const std::string& temp_dir)
    : output_dir_(output_dir) , temp_dir_(temp_dir), input_dir_(input_dir) {
    if (is_the_first_instance_) {
        fs::path dir(temp_dir_);
        if (fs::is_directory(dir)) {
            for (fs::directory_iterator it(dir); it != fs::directory_iterator(); ++it) {
                if (fs::is_regular_file(it->status())) {
                    fs::remove(it->path());
                }
            }
        }        is_the_first_instance_ = false;
        //then sets the flag for teh first run to false, so no more fiel deletions
    }
}

//takes a string representing the output and saves it to a file named 
//"output.txt" in the specified output directory. 
int FileManager::SaveOutput(const std::string& output) {
    std::istringstream input_stream(output);
    std::vector<std::pair<std::string, int>> word_counts;

    std::string line;
    while (std::getline(input_stream, line)) {
        std::istringstream line_stream(line);
        std::string word;
        int count;
        line_stream >> word >> count;
        //std::cout << "Word " << word << " count " << count << "\n";
        word_counts.emplace_back(word, count);
    }

    //Sorting word_counts from begin to teh end, The comparator function is a lambda function, defined using the [] syntax. 
    //It takes two parameters, a and b, which are the elements being compared during sorting. 
    //The auto keyword is used to let the compiler deduce the type of a and b automatically.

    std::sort(word_counts.begin(), word_counts.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
        });

    fs::path output_path(output_dir_);
    if (!fs::exists(output_path)) {
        //if there is no output dir with that name , write FAIL in the default TEMP
        //because if no output dir is specified, temp dir is probably also not specified
        std::cerr << "Output directory does not exist: " << output_dir_ << std::endl;
        FileManager fm(env_temp);
        std::cout << env_temp << "\n";
        fm.SaveResult("FAIL - no output dir - exit 2");
        return 2;
    }
    output_path /= "output.txt";
    //construct the full path to teh output file
    std::ofstream output_file(output_path.string());
    //if teh file us unable to open, locked, perms, etc, fail, Return Error caode 3, and later 
    //it will be saved in the specified TEMPFILE dir.
    if (!output_file.is_open()) {
        std::cerr << "Error creating output file " << output_path << std::endl;
        return 3;
    }
    for (const auto& word_count : word_counts) {
        //prints the pairs one by one in a file
        output_file << word_count.first << " " << word_count.second << "\n";
    }
    output_file.close();
    return 0;
}

//save the exit status , the result , Success or Fail string
void FileManager::SaveResult(const std::string& result) {
    fs::path output_path(output_dir_);
    if (!fs::exists(output_path)) {
        std::cerr << "Output directory does not exist: " << output_dir_ << std::endl;
        FileManager fm(env_temp);
        fm.SaveResult("FAIL - no output dir - exit 8");
        return ;
    }
    output_path /= "result.txt";
    std::ofstream output_file(output_path.string());
    if (!output_file.is_open()) {
        std::cerr << "Error creating result file " << output_path << std::endl;
        return;
    }
    output_file << result << "\n";
    output_file.close();
}



int FileManager::SaveTemp(const std::string& result , const std::string& filename) {
    fs::path temp_path(temp_dir_);
    if (!fs::exists(temp_path)) {
        std::cerr << "Temp directory does not exist: " << temp_dir_ << std::endl;
        //if there is no output dir with that name , write FAIL in the default TEMP
      //because if no output dir is specified, temp dir is probably also not specified
        FileManager fm(env_temp);
        fm.SaveResult("FAIL - no temp dir - 7 ");
        return 7;
    }

    temp_path /= filename;
    std::ofstream temp_file(temp_path.string());
    if (!temp_file.is_open()) {
        std::cerr << "Error creating temp file " << temp_path << std::endl;
        return 9;
    }
    temp_file << result << "\n";
    temp_file.close();
    return 0;
}
int FileManager::CheckDirs() {

    fs::path input_path(input_dir_);
    if (!fs::exists(input_path)) {
        std::cerr << "Input directory does not exist: " << input_dir_ << std::endl;
        return 5;
    }

    fs::path output_path(output_dir_);
    if (!fs::exists(output_path)) {
        std::cerr << "Output directory does not exist: " << output_dir_ << std::endl;
        return 8;
    }

    fs::path temp_path(temp_dir_);
    if (!fs::exists(temp_path)) {
        std::cerr << "Temp directory does not exist: " << temp_dir_ << std::endl;
        return 7;
            }

    return 0;

}
