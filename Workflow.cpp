#include "pch.h"

#include "Workflow.h"


namespace fs = boost::filesystem;

Workflow::Workflow(const std::string& inputDir, const std::string& outputDir , const std::string& tempDir ,
    const std::string& mapDll, const std::string& reduceDll,
    const std::string& mapFunc, const std::string& reduceFunc)
    : inputDir_(inputDir), outputDir_(outputDir) , tempDir_(tempDir) , reduceDll_(reduceDll),
    mapDll_(mapDll) , mapFunc_(mapFunc),  reduceFunc_(reduceFunc)  {}



int Workflow::CountWords() {
    int exit_code = 0;
    std::string buffer ;
    std::unordered_map<std::string, int> word_counts;


   HINSTANCE hDLL;               // Handle to DLL
    std::wstring wDllPath(mapDll_.begin(), mapDll_.end());
    LPCWSTR dllLib = wDllPath.c_str();
   hDLL = LoadLibrary(dllLib);
   int mapperLoadError = GetLastError();


    if (NULL != hDLL)
    {
      //  Mapper1* mapper;
        #define DLL_CALL __cdecl 
        //#define DLL_CALL __stdcall 

        typedef int (DLL_CALL *LPFNDLLFUNC1)(const std::string&, std::unordered_map<std::string, int>&);
        LPFNDLLFUNC1 lpfnDllMap = (LPFNDLLFUNC1)GetProcAddress(hDLL,  mapFunc_.c_str());
       // std::cout <<  GetLastError() << "\n";
       // std::cout << lpfnDllMap << " " << *lpfnDllMap << "\n";
        int mapperAddressError = GetLastError();

        if (NULL != lpfnDllMap)
        {

             FileManager i_fm(inputDir_);
             int R = 4;

    //for all the regular  files in teh input dir, run the mapper , and save teh Temp files into the 
    //temp dir with teh same names as input files

             for (const auto& file : fs::directory_iterator(inputDir_)) {

                if (!fs::is_regular_file(file.status())) {
                     continue;
                 }
     // std::cout << " before  While \n";
                int  num_words = i_fm.GetInput(file.path().string(), buffer);
                int end_pos = -1;
                for (int i = 0; i < R; i++) {
                    // calculate start and end positions for this part of the buffer
                    int start_pos = end_pos+1;
                    end_pos = (i + 1  == R ) ? (int)buffer.size() : (((i+1)* (int)buffer.size())/R);
                    // Ensure the substring ends at a space, to avoid splitting words
                    while (end_pos < buffer.size() && buffer[end_pos] != ' ') {
                        end_pos++;
                    }
                     // create a substring for this part of the buffer
                    std::string sub_buffer = buffer.substr(start_pos, end_pos - start_pos);
           // std::cout << " from While "<< empty  << buffer  << " \n";
                    word_counts.clear();
                    exit_code = lpfnDllMap(sub_buffer, word_counts);

           //std::cout << "another file " <<  entry.path().filename().string()  << " " << word_counts.size() << " \n";
                    FileManager fm(tempDir_);
                    std::string output;
           //composing the output for the temp file, splitting teh map entry 
           //into 2 parts and then stream it into teh file
                    for (auto& kv : word_counts) {
                        output += kv.first + ": " + std::to_string(kv.second) + "\n";
                    }
           //save the output into temp dir with teh same filename as teh file that ws processed 
                    exit_code = fm.SaveTemp(output);
           //exit code how teh Temp file was saved


                }

             }

        }
        else
        {
            // report the error
            exit_code = 12;
            std::cout << "cant get the address of teh func";
            BOOST_LOG_TRIVIAL(error) << "Error getting mapper  func address error: " << mapperAddressError;
        }
        FreeLibrary(hDLL);
    }
    else
    {
        exit_code = 13;

        std::cout << "cant load teh mapper dll";
        BOOST_LOG_TRIVIAL(error) << "Error loading mapper dll: " << mapDll_ <<" error: " << mapperLoadError;


    }

   // 
    word_counts.clear();
    

    //Reducer1 reducer(temp_dir_);
    //word_counts = reducer.reduce();
    if (exit_code == 0) {

        std::wstring  wDllPath1(reduceDll_.begin(), reduceDll_.end());
        dllLib = wDllPath1.c_str();
        hDLL = LoadLibrary(dllLib);
        int reducerLoadError = GetLastError();


        if (NULL != hDLL)
        {
            //  Mapper1* mapper;
#define DLL_CALL __cdecl 
//#define DLL_CALL __stdcall 


            typedef std::unordered_map<std::string, int>(DLL_CALL* LPFNDLLFUNC1)(const std::string&);

            LPFNDLLFUNC1 lpfnDllReduce = (LPFNDLLFUNC1)GetProcAddress(hDLL, reduceFunc_.c_str());
            int reducerAddressError = GetLastError();
            //  std::cout << lpfnDllReduce << " " << *lpfnDllReduce << "\n";

            if (NULL != lpfnDllReduce)
            {

                word_counts = lpfnDllReduce(tempDir_);
                //made it better with lambda function in "SaveOutput"
                //SortOutputByFrequency::SortOutput(word_counts);
                std::string output;
                for (auto& kv : word_counts) {
                    output += kv.first + ": " + std::to_string(kv.second) + "\n";

                }
                FileManager fm(outputDir_);
                exit_code = fm.SaveOutput(output);

            }
            else
            {
                // report the error
                std::cout << "can't get the address of the reduce func";
                BOOST_LOG_TRIVIAL(error) << "Error getting reduce func address error: " << reducerAddressError;
                exit_code = 14;

            }
            FreeLibrary(hDLL);
        }
        else
        {
            std::cout << "cant load the reducer dll";
            BOOST_LOG_TRIVIAL(error) << "Error loading reducer dll: " << reduceDll_ << " error: " << reducerLoadError;
            exit_code = 15;


        }

    }
 

     // Here the TEMPFILE dir should be cleaned, but we want it to survive for now for debuging purposes
    return exit_code;
}
