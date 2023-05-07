#include "pch.h"

#include <gtest/gtest.h>
#include "Workflow.h"
#include "FileManager.h"
#include <filesystem>
#include <boost/filesystem.hpp>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
namespace fs1 = std::filesystem;




TEST(WorkflowTest, CountWordsTest) {
    // Set up input and output directories
    boost::filesystem::remove_all("input-test");
    boost::filesystem::remove_all("output-test");
    boost::filesystem::remove_all("tempfiles-test");
    std::string input_dir = "input-test";
    std::string output_dir = "output-test";
    std::string temp_dir = "tempfiles-test";
    std::string mapDll = "\\libs\\Mapper.dll";
    std::string reduceDll = "\\libs\\Reducer1.dll";
    std::string mapFunc = "mapWrapper";
    std::string reduceFunc = "reduceWrapper";



    boost::filesystem::create_directory(input_dir);
    boost::filesystem::create_directory(output_dir);
    boost::filesystem::create_directory(temp_dir);



    // Create an input file
    std::string input_file_path = input_dir + "/input.txt";
    std::ofstream input_file(input_file_path);
    input_file << "apple\napple\napple apple \napple orange\nbanana apple\n";
    input_file.close();

    // Run the workflow and check the output
    Workflow workflow(input_dir, output_dir, temp_dir, mapDll, reduceDll, mapFunc, reduceFunc);
    int exit_code = workflow.CountWords();
    ASSERT_EQ(0, exit_code);

    std::ifstream output_file(output_dir + "/output.txt");
    std::string output((std::istreambuf_iterator<char>(output_file)),
        std::istreambuf_iterator<char>());
    ASSERT_EQ("apple: 6\norange: 1\nbanana: 1\n", output);
    output_file.close();
   boost::filesystem::remove_all("input-test");
   boost::filesystem::remove_all("output-test");
   boost::filesystem::remove_all("tempfiles-test");
}


TEST(WorkflowTest, CountWordsTestOneWord) {
    // Set up input and output directories
    boost::filesystem::remove_all("input-test");
    boost::filesystem::remove_all("output-test");
    boost::filesystem::remove_all("tempfiles-test");
    std::string input_dir = "input-test";
    std::string output_dir = "output-test";
    std::string temp_dir = "tempfiles-test";
    std::string mapDll = "\\libs\\Mapper.dll";
    std::string reduceDll = "\\libs\\Reducer1.dll";
    std::string mapFunc = "mapWrapper";
    std::string reduceFunc = "reduceWrapper";



    boost::filesystem::create_directory(input_dir);
    boost::filesystem::create_directory(output_dir);
    boost::filesystem::create_directory(temp_dir);



    // Create an input file
    std::string input_file_path = input_dir + "/input.txt";
    std::ofstream input_file(input_file_path);
    input_file << "apple\n";
    input_file.close();

    // Run the workflow and check the output
    Workflow workflow(input_dir, output_dir, temp_dir, mapDll, reduceDll, mapFunc, reduceFunc);
    int exit_code = workflow.CountWords();
    ASSERT_EQ(0, exit_code);

    std::ifstream output_file(output_dir + "/output.txt");
    std::string output((std::istreambuf_iterator<char>(output_file)),
        std::istreambuf_iterator<char>());
    ASSERT_EQ("apple: 1\n", output);
    output_file.close();
    boost::filesystem::remove_all("input-test");
    boost::filesystem::remove_all("output-test");
    boost::filesystem::remove_all("tempfiles-test");
}


TEST(WorkflowTest, CountWordsTestManyFiles) {
    // Set up input and output directories
    boost::filesystem::remove_all("input-test");
    boost::filesystem::remove_all("output-test");
    boost::filesystem::remove_all("tempfiles-test");
    std::string input_dir = "input-test";
    std::string output_dir = "output-test";
    std::string temp_dir = "tempfiles-test";
    std::string mapDll = "\\libs\\Mapper.dll";
    std::string reduceDll = "\\libs\\Reducer1.dll";
    std::string mapFunc = "mapWrapper";
    std::string reduceFunc = "reduceWrapper";



    boost::filesystem::create_directory(input_dir);
    boost::filesystem::create_directory(output_dir);
    boost::filesystem::create_directory(temp_dir);



    // Create an input files
    std::string input_file_path = input_dir + "/input1.txt";
    std::ofstream input_file(input_file_path);
    input_file << "apple\napple\napple apple \napple orange\nbanana apple\n";
    input_file.close();
     input_file_path = input_dir + "/input2.txt";
    std::ofstream input_file2(input_file_path);
    input_file2 << "apple $  apple orange @%$ \napple -orange\nbanana- -apple-\n";
    input_file2.close();

    // Run the workflow and check the output
    Workflow workflow(input_dir, output_dir, temp_dir, mapDll, reduceDll, mapFunc, reduceFunc);
    int exit_code = workflow.CountWords();
    ASSERT_EQ(0, exit_code);

    std::ifstream output_file(output_dir + "/output.txt");
    std::string output((std::istreambuf_iterator<char>(output_file)),
        std::istreambuf_iterator<char>());
    ASSERT_EQ("apple: 10\norange: 3\nbanana: 2\n", output);
    output_file.close();
     boost::filesystem::remove_all("input-test");
    boost::filesystem::remove_all("output-test");
     boost::filesystem::remove_all("tempfiles-test");
}



TEST(FileManagerTest, SaveOutputTest) {
    boost::filesystem::create_directory("output-test");

    // Initialize FileManager object with output directory
    FileManager fm("output-test");

    // Create a test string with some words and their counts
    std::string test_output = "apple 5\nbanana 1\norange 2\n";

    // Call SaveOutput function
    int result = fm.SaveOutput(test_output);

    // Check if function returns 0 for success
    EXPECT_EQ(result, 0);

    // Check if output file was created in the correct directory
    std::string output_file_path = "output-test\\output.txt";
    EXPECT_TRUE(std::filesystem::exists(output_file_path));

    // Read the output file and compare it with the test string, it should be sorted
    std::ifstream output_file(output_file_path);
    std::string line;
    std::getline(output_file, line);
    EXPECT_EQ(line, "apple 5");
    std::getline(output_file, line);
    EXPECT_EQ(line, "orange 2");
    std::getline(output_file, line);
    EXPECT_EQ(line, "banana 1");
    output_file.close();
    boost::filesystem::remove_all("output-test");

}

TEST(FileManagerTest, SaveTempTest) {
    boost::filesystem::create_directory("tempfiles-test");

    // Initialize FileManager object with temp directory
    FileManager fm("tempfiles-test");

    // Create a test string and a filename
    std::string test_output = "test output string";

    // Call SaveTemp function
    int result = fm.SaveTemp(test_output);

    // Check if function returns 0 for success
    EXPECT_EQ(result, 0);

    // Check if temp file was created in the correct directory with the correct name

    for (const auto& file : fs::directory_iterator("tempfiles-test")) {

        if (!fs::is_regular_file(file.status())) {
            continue;
        }

        std::string temp_file_path = file.path().string();
        EXPECT_TRUE(std::filesystem::exists(temp_file_path));

        // Read the temp file and compare it with the test string
        std::ifstream temp_file(temp_file_path);
        std::string line;
        std::getline(temp_file, line);
        EXPECT_EQ(line, "test output string");
        temp_file.close();
    }
    boost::filesystem::remove_all("tempfiles-test");

}








int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}