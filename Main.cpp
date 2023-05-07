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
#include <fstream>
#include "Workflow.h"
#include "FileManager.h"
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/program_options.hpp>

namespace keywords = boost::log::keywords;
namespace logging = boost::log;
namespace fs = boost::filesystem;
namespace po = boost::program_options;

int main_test(int argc, char** argv) {
    int exit_code = 0;
    std::string inputDir;
    std::string outputDir;
    std::string tempDir;
    std::string mapDll;
    std::string reduceDll;
    std::string mapFunc;
    std::string reduceFunc;




    logging::add_file_log("\\output_default\\mapreduce.log", keywords::open_mode = std::ios_base::app);
    std::time_t now = std::time(nullptr);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    std::stringstream file_name_ss;
    BOOST_LOG_TRIVIAL(info) << "Started At : " << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S");

    boost::program_options::options_description desc("Options");
    desc.add_options()
        ("help", "produce help message")
        ("input_dir", boost::program_options::value<std::string>(&inputDir)->required(), "input directory")
        ("output_dir", boost::program_options::value<std::string>(&outputDir)->default_value("\\output_default"), "output directory")
        ("temp_dir", boost::program_options::value<std::string>(&tempDir)->default_value("\\tempfiles_default"), "temp directory")
        ("map_dll", boost::program_options::value<std::string>(&mapDll)->default_value("\\libs\\Mapper.dll"), "map dll")
        ("reduce_dll", boost::program_options::value<std::string>(&reduceDll)->default_value("\\libs\\Reducer1.dll"), "reduce dll")
        ("map_func", boost::program_options::value<std::string>(&mapFunc)->default_value("mapWrapper"), "map func name")
        ("reduce_func", boost::program_options::value<std::string>(&reduceFunc)->default_value("reduceWrapper"), "reduce func name");

    po::variables_map vm;
 

    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 0;
        }

        po::notify(vm);
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    //std::cout << inputDir << " " << outputDir << " " << tempDir << " " << mapDll << " " << reduceDll << " " << mapFunc << " " << reduceFunc << " \n";
    BOOST_LOG_TRIVIAL(info) << inputDir << " " << outputDir << " " << tempDir << " " << mapDll << " " << reduceDll << " " << mapFunc << " " << reduceFunc << " ";


    FileManager fmtest(inputDir, outputDir, tempDir);
    exit_code = fmtest.CheckDirs();

    if (exit_code == 0) {
        Workflow wc(inputDir, outputDir, tempDir , mapDll , reduceDll , mapFunc , reduceFunc);
        exit_code = wc.CountWords();
    }

    FileManager fm(outputDir);
    if (exit_code == 0) {
        fm.SaveResult("Success");
        BOOST_LOG_TRIVIAL(info) << "Success.";
    }
    else {
        fm.SaveResult("FAIL " + std::to_string(exit_code));
        BOOST_LOG_TRIVIAL(error) << "Fail with error code  " << exit_code;
    }

    return exit_code;
}
