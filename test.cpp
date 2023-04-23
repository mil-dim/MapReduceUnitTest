#include "pch.h"

#include <gtest/gtest.h>
#include "Reducer.h"
#include "Workflow.h"
#include "FileManager.h"
#include "Mapper.h" 
#include <filesystem>
#include "SortOutputByFrequency.h"
#include <boost/filesystem.hpp>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
namespace fs1 = std::filesystem;

class MapperTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a test file with sample text
        std::ofstream file(test_file_path);
        file << "Hello, world! This is a test file.\n";
        file.close();
    }

    void TearDown() override {
        // Remove the test file
        std::remove(test_file_path.c_str());
    }

    std::string test_file_path = "test_file.txt";
};

TEST_F(MapperTest, MapTest) {
    Mapper mapper;
    std::unordered_map<std::string, int> word_counts;
    int ret = mapper.Map(test_file_path, word_counts);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(word_counts["hello"], 1);
    EXPECT_EQ(word_counts["world"], 1);
    EXPECT_EQ(word_counts["this"], 1);
    EXPECT_EQ(word_counts["is"], 1);
    EXPECT_EQ(word_counts["a"], 1);
    EXPECT_EQ(word_counts["test"], 1);
    EXPECT_EQ(word_counts["file"], 1);
}

TEST_F(MapperTest, CleanWordTest) {
    Mapper mapper;
    std::string word1 = "Hello,";
    std::string word2 = "world!";
    std::string word3 = "-This";
    std::string word4 = "is--is";
    std::string word5 = "a";
    std::string word6 = "test";
    std::string word7 = "file.";

    mapper.CleanWordWrapper(word1);
    mapper.CleanWordWrapper(word2);
    mapper.CleanWordWrapper(word3);
    mapper.CleanWordWrapper(word4);
    mapper.CleanWordWrapper(word5);
    mapper.CleanWordWrapper(word6);
    mapper.CleanWordWrapper(word7);

    EXPECT_EQ(word1, "hello");
    EXPECT_EQ(word2, "world");
    EXPECT_EQ(word3, "this");
    EXPECT_EQ(word4, "isis");
    EXPECT_EQ(word5, "a");
    EXPECT_EQ(word6, "test");
    EXPECT_EQ(word7, "file");
}

TEST(ReducerTest, ReduceTest) {
    // Create a temporary directory and write some files with word counts
    std::string temp_dir = "temp_dir";
    fs1::create_directory(temp_dir);
    std::ofstream file1(temp_dir + "/file1.txt");
    file1 << "apple:2" << std::endl;
    file1.close();
    std::ofstream file2(temp_dir + "/file2.txt");
    file2 << "orange:2" << std::endl;
    file2 << "apple:3" << std::endl;
    file2 << "banana:1" << std::endl;

    file2.close();

    // Call the reduce function and check the results
    Reducer reducer(temp_dir);
    std::unordered_map<std::string, int> word_counts = reducer.reduce();
    ASSERT_EQ(5, word_counts["apple"]);
    ASSERT_EQ(2, word_counts["orange"]);
    ASSERT_EQ(1, word_counts["banana"]);


    // Clean up the temporary directory
    fs::remove_all(temp_dir);
}


TEST(SortOutputByFrequencyTest, SortOutputTest) {
    // Create an unordered_map with some word counts
    std::unordered_map<std::string, int> word_counts = {
      {"apple", 2},
      {"orange", 1},
      {"banana", 3}
    };

    // Sort the word counts and check the result
    SortOutputByFrequency sorter;
    sorter.SortOutput(word_counts);
    ASSERT_EQ(3, word_counts["banana"]);
    ASSERT_EQ(2, word_counts["apple"]);
    ASSERT_EQ(1, word_counts["orange"]);
}



TEST(WorkflowTest, CountWordsTest) {
    // Set up input and output directories
    std::string input_dir = "C:\\Users\\d_mil\\OneDrive\\Escritorio\\OOD\\shakespeare-test";
    std::string output_dir = "D:\\output-test";
    std::string temp_dir = "D:\\tempfiles-test";

    // Create an input file
    std::string input_file_path = input_dir + "/input.txt";
    std::ofstream input_file(input_file_path);
    input_file << "apple orange\nbanana apple\napple\n";
    input_file.close();

    // Run the workflow and check the output
    Workflow workflow(input_dir, output_dir, temp_dir);
    int exit_code = workflow.CountWords();
    ASSERT_EQ(0, exit_code);

    std::ifstream output_file(output_dir + "/output.txt");
    std::string output((std::istreambuf_iterator<char>(output_file)),
        std::istreambuf_iterator<char>());
    ASSERT_EQ("apple: 3\norange: 1\nbanana: 1\n", output);
}




TEST(FileManagerTest, SaveOutputTest) {
    // Initialize FileManager object with output directory
    FileManager fm("D:\\output-test");

    // Create a test string with some words and their counts
    std::string test_output = "apple 5\nbanana 1\norange 2\n";

    // Call SaveOutput function
    int result = fm.SaveOutput(test_output);

    // Check if function returns 0 for success
    EXPECT_EQ(result, 0);

    // Check if output file was created in the correct directory
    std::string output_file_path = "D:\\output-test\\output.txt";
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
}

TEST(FileManagerTest, SaveTempTest) {
    // Initialize FileManager object with temp directory
    FileManager fm("", "", "D:\\tempfiles-test");

    // Create a test string and a filename
    std::string test_output = "test output string";
    std::string filename = "test_file.txt";

    // Call SaveTemp function
    int result = fm.SaveTemp(test_output, filename);

    // Check if function returns 0 for success
    EXPECT_EQ(result, 0);

    // Check if temp file was created in the correct directory with the correct name
    std::string temp_file_path = "D:\\tempfiles-test\\test_file.txt";
    EXPECT_TRUE(std::filesystem::exists(temp_file_path));

    // Read the temp file and compare it with the test string
    std::ifstream temp_file(temp_file_path);
    std::string line;
    std::getline(temp_file, line);
    EXPECT_EQ(line, "test output string");
}





// Declare the main function so we can test it
int main_test(int argc, char** argv);

// Test the main function with valid arguments
TEST(MainTest, ValidArguments) { 
  /*  // Create a test input directory
    boost::filesystem::path input_dir = boost::filesystem::unique_path();
    boost::filesystem::create_directory(input_dir);
    std::ofstream((input_dir / "file1.txt").string()) << "word1: 1\nword2: 2\n";
    std::ofstream((input_dir / "file2.txt").string()) << "word3: 3\nword4: 4\n";

    // Create a test output directory
    boost::filesystem::path output_dir = boost::filesystem::unique_path();
    boost::filesystem::create_directory(output_dir);



    // Create a test temporary directory
    boost::filesystem::path temp_dir = boost::filesystem::unique_path();
    boost::filesystem::create_directory(temp_dir);

    */
    
    // Call the main function with test arguments
    char* argv[] = { "main_test", "C:\\Users\\d_mil\\OneDrive\\Escritorio\\OOD\\shakespeare1" , "D:\\output" ,"D:\\tempfiles"};

 int exit_code = main_test(sizeof(argv) / sizeof(char*), argv);

    // Check that the exit code is 0, indicating success
    EXPECT_EQ(exit_code, 0);


  /*
   // Check that the output file was created and contains the expected content
    std::string output_path = (output_dir / "output.txt").string();
    std::ifstream output_file(output_path);
    std::stringstream expected_output;
    expected_output << "word4 4\nword3 3\nword2 2\nword1 1\n";
    std::stringstream actual_output;
    actual_output << output_file.rdbuf();
    EXPECT_EQ(expected_output.str(), actual_output.str()); */
    // Remove the test directories
  //  boost::filesystem::remove_all(input_dir);
   // boost::filesystem::remove_all(output_dir);
   // boost::filesystem::remove_all(temp_dir);
}

TEST(MainTest, InValidInputDir) {
    // Call the main function with test arguments
    char* argv[] = { "main_test", "C:\\Users\\d_mil\\OneDrive\\Escritorio\\OOD\\shakespeare-bad" , "D:\\output" ,"D:\\tempfiles" };

    int exit_code = main_test(sizeof(argv) / sizeof(char*), argv);

    // Check that the exit code is 0, indicating success
    EXPECT_EQ(exit_code, 5);
}

// Test the main function with invalid arguments
TEST(MainTest, InvalidArguments) {
    // Call the main function with no arguments
    char* argv[] = { "main_test" };
    int exit_code = main_test(sizeof(argv) / sizeof(char*), argv);

    // Check that the exit code is 1, indicating failure
    EXPECT_EQ(exit_code, 1);
}

TEST(MainTest, InValidOutputDir) {
    // Call the main function with test arguments
    char* argv[] = { "main_test", "C:\\Users\\d_mil\\OneDrive\\Escritorio\\OOD\\shakespeare1" , "D:\\output-bad" ,"D:\\tempfiles" };

    int exit_code = main_test(sizeof(argv) / sizeof(char*), argv);

    // Check that the exit code is 0, indicating success
    EXPECT_EQ(exit_code, 8);
}
TEST(MainTest, InValidTempDir) {
    // Call the main function with test arguments
    char* argv[] = { "main_test", "C:\\Users\\d_mil\\OneDrive\\Escritorio\\OOD\\shakespeare1" , "D:\\output" ,"D:\\tempfiles-bad" };

    int exit_code = main_test(sizeof(argv) / sizeof(char*), argv);

    // Check that the exit code is 0, indicating success
    EXPECT_EQ(exit_code, 7);
}
int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}