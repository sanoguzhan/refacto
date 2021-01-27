#include "dtables.hpp"
#include <vector>
#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>

using json = nlohmann::json;

void keys(std::string path){
        std::vector<std::string> vec;
        YAML::Node file = YAML::LoadFile(path);
        for(const auto& kv: file){
            // vec.push_back(kv.first.as<std::string>());
            std::cout << kv.first << "\n";
        }

}


void print(){
    std::cout<< "tetsts" << std::endl;
}