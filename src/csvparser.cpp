#include "csvparser.hpp"




string CSVParser::validate_f(string path){
        fs::path p(path);
        if(fs::exists(p)){
            return path;
    }
    throw std::runtime_error("CSV not exist at " +  file_path);
}
