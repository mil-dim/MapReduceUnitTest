#ifndef WORDCOUNTER_H
#define WORDCOUNTER_H

#include <string>

#include "FileManager.h"
#include "windows.h"

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

#include <boost/uuid/uuid_io.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
class Workflow {
public:
    Workflow(const std::string& input_dir, const std::string& output_dir, const std::string& temp_dir ,
        const std::string& mapDll, const std::string& reduceDll, 
        const std::string& mapFunc, const std::string& reduceFunc  );

    int  CountWords();

private:
    std::string inputDir_ ;
    std::string outputDir_ ;
    std::string tempDir_ ;
    std::string reduceDll_ ;
    std::string mapDll_ ;
    std::string mapFunc_ ;
    std::string reduceFunc_ ;
};

#endif // WORDCOUNTER_H
