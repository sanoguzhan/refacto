#include "dtables.hpp"


using namespace table;


typedef std::map<std::string, std::vector<std::string>>::iterator map_iter;
typedef std::vector<std::pair<string, std::vector<string>>> pair_vector;

bool Table::insert(Series series){

    // std::map<std::string, std::pair<std::string, std::vector<string>>>data; // Data contains each column as DataRow Obj


    std::pair<string, std::vector<std::string>> row;
    for(const auto& p: series.values){
        if(data.find(p.first) == data.end()){
            data.insert({
            p.first, pair_vector{make_pair(series.name, p.second)}
        });
        }else if(column_exist(p.first, series.name)){
           // Insert colum 
        }

        
    }

    return true;

}


bool Table::column_exist(const string& id,const string& col_name)const{
    for(const auto& p: data.at(id)){
        if(col_name == p.first){
            return true;
        }
    }
    return false;
}