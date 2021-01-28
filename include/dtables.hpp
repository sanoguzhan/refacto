#ifndef __DTABLES_HPP
#define __DTABLES_HPP


#include<iostream>
#include<string>
#include<vector>
#include<variant>
#include<filesystem>
#include <utility>  
#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>

std::variant<int, float, std::string> v;

namespace dtable{
    namespace fs = std::filesystem;

    class Table{

        public:
            std::string path;
            YAML::Node file;
            
            std::vector<std::pair<std::string,std::vector<std::string>>> data;
        
        Table(std::string);
        std::vector<std::string> columns();

        private:
            YAML::Node open_yaml(std::string);
            void validate();
            void insert();
        };

}
#endif