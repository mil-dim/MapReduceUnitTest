#ifndef WORKFLOW_H
#define WORKFLOW_H

#include <string>
#include "Mapper.h"
#include "Reducer.h"
#include "SortOutputByFrequency.h"
#include "FileManager.h"

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
class Workflow {
public:
    Workflow(const std::string& input_dir, const std::string& output_dir, const std::string& temp_dir);

    int  CountWords();

private:
    std::string input_dir_;
    std::string output_dir_;
    std::string temp_dir_;

};

#endif // WORKFLOW_H
