#include "dtables.hpp"


using json = nlohmann::json;
using namespace dtable;



std::vector<std::string> Table::columns(){
        std::vector<std::string> vec;
        
        const YAML::Node& columns = file["COLUMNS"];
         
        for(YAML::const_iterator it = columns.begin(); it != columns.end(); it++ ){
            for(auto& kv:*it){
                vec.push_back(kv.first.as<std::string>());
            }
        }
    return vec;
}

dtable::Table::Table(std::string path): 
    path{path},file{open_yaml(path)}{
    validate();
    insert();
}



YAML::Node Table::open_yaml(std::string file_path){
    fs::path p(file_path);
    if(fs::exists(p)){
        return YAML::LoadFile(p);
    }
    throw std::runtime_error("File not exist at " +  file_path);
}

void Table::insert(){
   const YAML::Node& columns = file["COLUMNS"];
         
    for(YAML::const_iterator it = columns.begin(); it != columns.end(); it++ ){
        for(auto& kv:*it){
            if(kv.second.as<std::string>() == "datetime"){
                std::vector<std::string> vec;
                std::pair<std::string, std::vector<std::string>> mp = {kv.first.as<std::string>(), vec};
                data.push_back(mp);
            }
            // vec.push_back((std::string)kv.first.as<std::string>());
        }

        for(auto e:data){
            std::cout << e.first << std::endl;
        }
    } 

}

void Table::validate(){
    
    
    if(!file["COLUMNS"]){
        throw std::runtime_error("Configuration missing TABLE ");
    }
}

