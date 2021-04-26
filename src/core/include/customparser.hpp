/** @file csvparser.hpp
 *  @brief Custom Parser csv operator for data accessing and structring
 *
 *  This file contains the refacto's csvparser
 *  
 * 
 *  @todo Column-wise token search and parsing
 * 
 *  @author Oguzhan San
 *  @bug No known bugs.
 * 
 */

#ifndef __CUSTOMPARSER_HPP
#define __CUSTOMPARSER_HPP

#include <iostream>
#include <string>
#include <filesystem>
#include <regex>
#include <fstream>
#include <map>
#include <utility>
#include <vector>
#include <glob.h>
#include <algorithm>
#include <cctype>
#include<deque>
#include <boost/log/trivial.hpp>
#include "table.hpp"
#include "parser.hpp"

#if _WIN32 || _WIN64
#define NAV "\"
#else
#define NAV "/"
#endif

using namespace std;
using namespace aria;
using namespace table;
// namespace fs = std::filesystem;

using u_vector = const std::vector<u_int32_t>;
using s_matrix = const std::vector<std::vector<string>>;

using std::vector;
using std::string;
using std::map;
using std::deque;
using std::shared_ptr;
/**
 * @brief Loc struct for paramaterization of variables
 * 
 * Loc Struct
 * 
 *  Variable search object, used to locate variables
 *  
 *  name: (string) name of variable which is searched
 *  orient: (string) row or column
 *  row: (int) row location
 *  column: (int) column location
 */
struct Entity
{
    string key;  //meta info ( example: inverter)
    string name;
    string orient;
    string type;
    int value_begin;
    vector<map<string,string>> conditions;

    int row;
    int column;
    int from;
    int to;

    string keyword; // multi or filename


    Entity(string key, string name, string type, string keyword)
        : key{key}, name{name}, type{type}, keyword{keyword}{}
    // For Vector Insertation
    Entity(string key, 
            string name, 
            string type,
            int value_begin,
            int from,
            int to)
            : key{key}, name{name}, type{type}, 
                value_begin{value_begin}, from{from}, to{to} {}

    // For id initilization
    Entity(string key, 
            string name,
            string orient,
            string type, 
            int row, 
            int column, 
            int value_begin,
            string keyword= "multi")
            : key{key}, name{name}, orient{orient}, type{type}, row{row},
                column{column}, value_begin{value_begin}, keyword{keyword}
            {}

    Entity(string key, string name, string orient, string type,int row, int value_begin=0)  
            :key{key}, name{name},orient{orient}, type{type},row{row}, value_begin{value_begin}
                {}
    Entity(string key, string name, string orient,string type, 
             vector<map<string,string>> conditions, int value_begin=0)  
            :key{key}, name{name},orient{orient}, type{type}, 
                value_begin{value_begin}, conditions{conditions}{}

    friend bool operator>(const Entity &right, const Entity &left);
    friend bool operator<=(const Entity &right, const Entity &left);
    friend bool operator<(const Entity &right, const Entity &left);
    friend bool operator>=(const Entity &right, const Entity &left);
    
};


deque<string> get_flist(string pattern){
    pattern += "*.csv";
    glob_t glob_result;
    glob(pattern.c_str(),GLOB_TILDE,NULL,&glob_result);
    deque<string> files;
    for(unsigned int i=0;i<glob_result.gl_pathc;++i){
        files.push_back(string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return files;
}

/**
 * @brief CSVParser Class for CSV Data Search
 *  CSVParser Class
 *      Opens file
 *      Search given data
 */
class CustomParser
{
private:
    // string file_path;
    // std::ifstream f;
    // int skip_rows = 0;
    // // csv::CsvParser parser;
    // std::string delim = ";";
    // string dir_path;
    // deque<string> files_vector;
    // map<string,shared_ptr<Table>> tables;

    vector<Entity> meta;
    
public:
    /**
     * @brief resulting data of the parsing composed of 2D array with 1th axis as row and 2nd as column
     * 
     */
    std::vector<std::vector<string>> data;
    std::map<string, shared_ptr<Table>> tables;
    /**
     * @brief filename of the parsed csv file 
     * 
     */

    std::string file_name;
    template<typename ...T>
    CustomParser(T ...lst)
        : meta{lst...}{}



    
    void operator()(string dir_path, string delimeter, int skip){
        deque<string> files{get_flist(dir_path)};
        map<string, vector<Entity>> keys;
        transform_keys(keys);
        Series series;
        vector<string> vec_values;

        for(auto file: files){
            #ifdef LOG
            BOOST_LOG_TRIVIAL(info) << "Reading file: " << file <<std::endl;
            #endif
            in_read(file, delimeter, skip);
            file_name = get_substring("/", ".", file);
        }
        for(auto p:tables){
            in_insert(p.second, keys.at(p.first), series, vec_values);
        }
    }

    void in_insert(shared_ptr<Table> tb, 
            const vector<Entity> &entity_lst,
            Series &series,
            vector<string> &vec_values){
        for(auto item:entity_lst){
            if(item.type == "ids" && !ids_exists){
                create_ids(series, item);
            }
            else if(item.type == "series"){
                from_series(series, item);
                tb->insert(series);
                series.values.clear(); // clean the parsed values
            }else if(item.type == "vector"){
                from_vector(vec_values, item);
                tb->insert(item.name, vec_values);
                vec_values.clear();
            }else if(item.type == "entity"){
                if(item.keyword =="file_name")
                    tb->insert(item.name, file_name);
                else{
                    tb->insert(item.name, item.keyword);
                }
            }else{
                throw std::runtime_error("Wrong Configuration identifier!");
            }
        }
    }

 void from_vector(vector<string> &vec_values, 
                    Entity& item){
    
    int32_t counter = 0;
    if (item.orient == "row"){
        if (item.to == -1)
            item.to = data.at(item.value_begin).size();
        vec_values.insert(vec_values.end(), 
                        data.at(item.value_begin).begin() + item.from, 
                        data.at(item.value_begin).begin() + item.to);
    }
    else if (item.orient == "column"){
        if (item.to == -1) item.to = data.size();
        for (const auto &col : data){
            if (counter == item.to) break;
            if (counter >= item.from) vec_values.push_back(col.at(item.value_begin));
            counter++;
        }
    }
    else{throw std::runtime_error("Orient is missing!");}
    }

    private:
        bool ids_exists{false};


        void from_series(Series &series, const Entity& item){
             if(item.conditions.empty()) {
                 construct_series(series, item);
             }
           else if(item.conditions.size() == 1){
                construct_series(series, item, item.conditions.at(0));
            }
            else if(item.conditions.size() == 2){
                throw std::runtime_error("Multi conditions are not supported");
            }else{
                throw std::runtime_error("Wrong configurations");
            }
        }
     
       
        void from_value(const Entity& item){

        }


    void create_ids(Series& series, const Entity& item){
        if(item.keyword == "file_name"){
            series.values.insert(std::make_pair(file_name, vector<string>()));
        }else if(item.keyword == "single"){
            series.values.insert(std::make_pair(data.at(item.row).at(item.column), vector<string>()));
        }else if(item.keyword == "multi"){
            vector<u_int32_t> indexes;
            find_ids(data,indexes, item);
            for(auto i:indexes){
                series.values.insert(std::make_pair(std::to_string(i), vector<string>()));
            }
            indexes.clear();
        }else{
            throw std::runtime_error("Wrong id config");
        }
    }


   void construct_series(Series& series,
                       const Entity& item){
    std::vector<string> rows;

    //First Condition as name
    series.name = item.name;

    std::vector<u_int32_t> data_idx;
    find_ids(data,data_idx,item);

    if(data_idx.size() != series.values.size()){
        throw std::runtime_error(R"(IDs not initilized correctly, 
                                There are more data column than ids)");
    }
    std::map<string,vector<string>>::iterator current_id = series.values.begin();
    for (const u_int32_t col : data_idx){
        for (size_t i = item.value_begin; i < data.size(); i++){
            rows.push_back(data.at(i).at(col));
        }

        //Iterate with keys
        series.values.find(current_id->first)->second.insert(
                std::end(series.values.find(current_id->first)->second),
                std::begin(rows),
                std::end(rows));
        rows.resize(0);
        }
        current_id++;
    }


   void construct_series(Series& series,
                       const Entity& item,
                        map<string, string> target){
    std::vector<string> rows;

    //First Condition as name
    series.name = item.name;

    std::vector<u_int32_t> data_idx;
    string current_id;
    // Conditional id search
    find_ids(data,data_idx, target, item);
    for (const u_int32_t col : data_idx){
        for (size_t i = item.value_begin; i < data.size(); i++){
            rows.push_back(data.at(i).at(col));
        }
        current_id =data.at(std::stoi(target.find("row")->second)).at(col);

        //Key not Exist so Initilize
        if(series.values.find(current_id) == series.values.end()){
            series.values.insert({current_id, rows});
        }
        else{
            series.values.find(current_id)->second.insert(
                std::end(series.values.find(current_id)->second),
                std::begin(rows),
                std::end(rows)
            );
        }
        rows.resize(0);
        }
    }



    void in_read(const string& path,  string delim, int skip_rows){
        
        std::ifstream file(path);
        csv::CsvParser p{file};
        
        p.delimiter(delim[0]);
        int to_skip = 0;

        for (auto iter_row = p.begin(); iter_row != p.end(); iter_row++){
            if (to_skip < skip_rows){
                to_skip++;
                continue;
            }
            data.push_back(*iter_row);
        }
    }



    void transform_keys(map<string, vector<Entity>> &keys){
        vector<string> meta_names;
        std::sort(meta.begin(), meta.end());
        std::transform(meta.begin(), meta.end(), back_inserter(meta_names), [](Entity &id){
            return id.key;
        });
        meta_names.erase(unique(meta_names.begin(), meta_names.end()), meta_names.end());

        // Groups all entities by key name
        transform(meta_names.begin(), meta_names.end(), std::inserter(keys, keys.end()), [](string &key){
            return std::make_pair(key, vector<Entity>());
        });

        // Creates and group tables by key name
        transform(meta_names.begin(), meta_names.end(), std::inserter(tables, tables.end()), [](string &key){
            return std::make_pair(key, make_shared<Table>());
        });

        for_each(meta.begin(), meta.end(), [&](Entity& id){
            keys.at(id.key).emplace_back(id);
        });
    }


void find_ids(s_matrix &data,
                vector<u_int32_t> &indexes,
                 const Entity &item){
    string lookup_id;
    std::regex reg_id{item.name};
    for (size_t c = 0; c < data.at(0).size(); c++){
        lookup_id = data.at(item.row).at(c);
        if(std::regex_match ( lookup_id, reg_id) )
            indexes.push_back(c);
        }
    }


void find_ids(s_matrix &data,
                vector<u_int32_t> &indexes,
                 map<string,string> cond,
                 const Entity &item){
    string lookup_var, lookup_id;
    std::regex reg_id{cond.find("name")->second};
    for (size_t c = 0; c < data.at(0).size(); c++){
        lookup_id = data.at(std::stoi(cond.find("row")->second)).at(c);
        lookup_var =data.at(item.row).at(c);
        if(std::regex_match ( lookup_id, reg_id) && 
            lookup_var.find(item.name) != string::npos)
            indexes.push_back(c);
        }
    }


string get_substring(std::string delimiter, std::string extention, std::string s)
{
    std::string token;
    size_t pos = 0;
    while ((pos = s.find(delimiter)) != std::string::npos)
    {
        token = s.substr(0, pos);
        s.erase(0, pos + delimiter.length());
    }
    pos = 0;
    while ((pos = s.find(extention)) != std::string::npos)
    {
        token = s.substr(0, pos);
        s.erase(pos, token.length() + delimiter.length());
    }
    return s;
}

};











inline bool operator>(const Entity &right, const Entity& left){
    return right.key > left.key;
}
inline bool operator<=(const Entity& right, const Entity& left){
    return right.key <= left.key;
}
inline bool operator<( const Entity& right, const Entity& left){
    return right.key < left.key;
}

inline bool operator>=(const Entity& right, const Entity& left){
    return right.key >= left.key;
}

#endif