
#ifndef REDUCER_H
#define REDUCER_H

#include <string>
#include <unordered_map>
#include <fstream>
#include <filesystem>



class Reducer {
public:
    Reducer(const std::string& temp_dir);
    std::unordered_map<std::string, int> reduce();

private:
    std::string temp_dir_;
};

#endif // REDUCER_H

