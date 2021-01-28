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
    std::string column;
    for(YAML::const_iterator it = columns.begin(); it != columns.end(); it++ ){
        for(auto& kv:*it){
            column = kv.second.as<std::string>();
            if(compare_column(column, "datetime")){
                auto row = std::make_shared<DataRow>();
                row->Dtype = "datetime";                
                row->String.first = kv.first.as<std::string>();                // std::vector<std::string> vec;
                data.push_back(std::move(row));
            }
            else if(compare_column(column, "float")){
                        auto row = std::make_shared<DataRow>();
                        row->Dtype = "double";                
                        row->Float.first = kv.first.as<std::string>();                // std::vector<std::string> vec;
                        data.push_back(std::move(row));
                        
                    }
            else if(compare_column(column, "int")){
                        auto row = std::make_shared<DataRow>();
                        row->Dtype = "int";                
                        row->Int.first = kv.first.as<std::string>();                // std::vector<std::string> vec;
                        data.push_back(std::move(row));
                        
                    }
        
        }

    } 

}

bool Table::compare_column(const std::string f, std::string s){
    std::string capitalized_s = s;
    std::string upper_s = s;    
    capitalized_s[0] = std::toupper(capitalized_s[0]);

    std::transform(capitalized_s.begin()+1,
                    capitalized_s.end(), 
                    capitalized_s.begin()+1, ::tolower);
    std::transform(upper_s.begin()+1,
                    upper_s.end(), 
                    upper_s.begin()+1, ::toupper);
    
    if(f == s 
        || f == upper_s 
        || f == capitalized_s){
            return true;
        }
        return false;
}

void Table::validate(){
    
    if(!file["COLUMNS"]){
        throw std::runtime_error("Configuration missing COLUMNS ");
    }
}

