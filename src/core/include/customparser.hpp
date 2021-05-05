/** @file csvparser.hpp
 *  @brief Custom Parser csv operator for data accessing and structring
 *
 *  This file contains the refacto's customparser
 *  includes id search and variable insertion
 *  
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

#define NAV "/"

using namespace std;
using namespace aria;
using namespace table;

using u_vector = const std::vector<u_int32_t>;
using s_matrix = const std::vector<std::vector<string>>;

using std::vector;
using std::string;
using std::map;
using std::deque;
using std::shared_ptr;


/**
 * @brief Entity struct for paramaterization of variables
 * 
 * Entity Struct
 * 
 *  Variable search object, used to locate variables
 *
 * Used for
 *  - ID Search-create
 *  - Variable seach
 *  - Vector Search
 *  - Single value search
 *  
 */


struct Entity{
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

     // For single value insertion
    Entity(string key, string name, string type, string keyword)
        : key{key}, name{name}, type{type}, keyword{keyword}{}

    // For Vector Insertation
    Entity(string key, string name, string orient,
            string type, string keyword, int column,
            int from,int to)
            : key{key}, name{name}, orient{orient}, type{type}, keyword{keyword},
            column{column}, from{from}, to{to} {}

    // For id initilization
    Entity(string key, string name, string orient, string type, 
            int row, int column, int value_begin,
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


void find_ids(s_matrix &,vector<u_int32_t> &,const Entity &);
void find_ids(s_matrix &,vector<u_int32_t> &,map<string,string>, const Entity &);

/**
 * @brief Get the flist object
 * 
 * @param pattern "file extension"
 * @return deque<string> 
 */
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
 * @brief CustomParser Class for CSV Data Search
 *  CSVParser Class
 *      Opens file
 *      Search given data
 */
class CustomParser{
    private:

        vector<Entity> meta;
        
    public:
        /**
         * @brief resulting data of the parsing composed of 2D array with 1th axis as row and 2nd as column
         * 
         */
        std::vector<std::vector<string>> data;
        std::map<string, shared_ptr<Table>> tables;
            
        template<typename ...T>
        CustomParser(T ...lst)
            : meta{lst...}{}

    private:
        string id_iter, id_type;
        vector<string> id_iter_vector;
        std::string file_name;


    public:

        /**
         * @brief Overload ()operator for hyperparameters
         * 
         * @param dir_path 
         * @param delimeter 
         * @param skip 
         */
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
                for(auto p:tables){
                    in_insert(p.second, keys.at(p.first), series, vec_values);
                    // p.second->insert(series);
                    series.values.clear(); // clean the parsed values
                    id_iter_vector.clear();
                }
                data.clear();
            }
        }

        /**
         * @brief Insert series to tables
         * 
         *  Parse values to series
         *  insert series to table 
         *  flush series for each entity
         * 
         * 
         * @param tb (shared_ptr<Table>) 
         * @param entity_lst (const vector<Entity> &) conditional search 
         * @param series (Series &) series to be inserted 
         * @param vec_values (vector<string> &) vector values to be inserted
         */
        void in_insert(shared_ptr<Table> tb, 
                const vector<Entity> &entity_lst,
                Series &series,
                vector<string> &vec_values){


            for(auto item:entity_lst){
                if(item.type == "ids"){
                    create_ids(series, item);
                }
                else if(item.type == "series"){
                    from_series(series, item, tb);
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

        /**
         * @brief Insert values to vector
         * 
         *  Parse values vec_values
         * @param item ( Entity&) conditional search 
         * @param vec_values (vector<string> &) vector values to be inserted
         */
        void from_vector(vector<string> &vec_values, 
                        Entity& item){
        
            int32_t counter = 0;
            if(item.column > data.size())
                throw std::runtime_error("Column index is bigger than data index");
            if (item.orient == "row"){
                if (item.to == -1) item.to = data.at(item.column).size();
                vec_values.insert(vec_values.end(), 
                                data.at(item.column).begin() + item.from, 
                                data.at(item.column).begin() + item.to);
            }
            else if (item.orient == "column"){
                if (item.to == -1) item.to = data.size();
                for (const auto &col : data){
                    if (counter == item.to) break;
                    if (counter >= item.from) vec_values.push_back(col.at(item.column));
                    counter++;
                }
            }
            else{throw std::runtime_error("Orient is missing!");}
        }


        /**
         * @brief writes to csv
         * 
         * @param dir (string) directory path 
         */
        void to_csv(string dir){
            string save_path;
            for(const auto& p:tables){
                save_path = dir + "/" + p.first + ".csv";
                p.second->save(save_path);
            }
        }

    private:

        /**
         * @brief Creates series with entity condition
         * 
         * @param series (Series) series which will be filled
         * @param item (Entity) condition for series construction
         * @param tb (shared_pr<Table>) table which data will be inserted
         */
        void from_series(Series &series, const Entity& item, 
                    shared_ptr<Table> tb){
             if(item.conditions.empty()) {
                 construct_series(series, item, tb);
             }
           else if(item.conditions.size() == 1){
               std::cout << "here" <<std::endl;
                construct_series(series, item, item.conditions.at(0), tb);
            }
            else if(item.conditions.size() == 2){
                throw std::runtime_error("Multi conditions are not supported");
            }else{
                throw std::runtime_error("Wrong configurations");
            }
        }
     
    /**
     * @brief Create a ids and initilize map keys
     * 
     * @param series (Series&) series for id initilization
     * @param item (Entity) id search condition
     */
    void create_ids(Series& series, const Entity& item){
        if(item.keyword == "file_name"){
            if(!id_exist(series, file_name)){
                series.values.insert(std::make_pair(file_name, vector<string>()));
                id_type = "single";
                id_iter = file_name;
            }
        }else if(item.keyword == "single"){
            if(!id_exist(series, data.at(item.row).at(item.column))){
                series.values.insert(std::make_pair(data.at(item.row).at(item.column), vector<string>()));
                id_type = "single";
                id_iter = data.at(item.row).at(item.column);
            }
        }else if(item.keyword == "multi"){
            vector<u_int32_t> idx_vector;
            find_ids(data,idx_vector, item);
            for(auto i:idx_vector){
                if(!id_exist(series, data.at(item.value_begin).at(i))){
                   id_iter_vector.push_back(data.at(item.value_begin).at(i));
                   series.values.insert(std::make_pair(data.at(item.value_begin).at(i), vector<string>()));
                }
            }
            id_type = "multi";
            idx_vector.clear();
        }else{
            throw std::runtime_error("Wrong id config");
        }
    }

    /**
     * @brief Checks if id exists 
     * 
     * @param series (Series&) Series id will be checked
     * @param check_id (string) id to be checked
     * @return true if exists
     * @return false if not
     */
    bool id_exist(Series& series, string check_id){
    
        if(series.values.find(check_id) == series.values.end())
            return false;
        return true;
    }

    /**
     * @brief Construct series for insertion
     * 
     * @param series (Series&) series for construction
     * @param item (Entity&) Search conditions
     * @param tb (shared_ptr<Table>) tables for insertion
     */
   void construct_series(Series& series,
                       const Entity& item,
                       shared_ptr<Table> tb){
    vector<string> rows;

    //First Condition as name
    series.name = item.name;
    std::vector<u_int32_t> data_idx;
    find_ids(data,data_idx,item);
    if(series.values.empty())
        throw std::runtime_error("ID not initilized correctly!");    
    if(id_type == "single"){
        for (const u_int32_t col : data_idx){
            for (size_t i = item.value_begin; i < data.size(); i++){
                rows.push_back(data.at(i).at(col));
            }
            series.values.find(id_iter)->second.insert(
                    std::end(series.values.find(id_iter)->second),
                    std::begin(rows),
                    std::end(rows));
            rows.resize(0);
            }
            tb->insert(series);
    }else if(id_type == "multi"){
        if(id_iter_vector.size() != data_idx.size())
            throw std::runtime_error("ID size not matched with variables");
        std::vector<string>::iterator id_iterator{id_iter_vector.begin()};
        for (const u_int32_t col : data_idx){
            for (size_t i = item.value_begin; i < data.size(); i++){
                rows.push_back(data.at(i).at(col));
            }
            series.values.find(*id_iterator)->second.insert(
                    std::end(series.values.find(*id_iterator)->second),
                    std::begin(rows),
                    std::end(rows));
            rows.resize(0);
            id_iterator++;

            }
            tb->insert(series);
    }
    clean_series(series);
    }

    void inline clean_series(Series& series){
        for(auto& p:series.values){
            series.values.find(p.first)->second.clear();
        }
    }

    /**
     * @brief Construct series for insertion with condition
     * 
     *  - Conditional id search
     * 
     * @param series (Series&) series for construction
     * @param item (Entity&) Search conditions
     * @param target (map<string, string>) condition
     * @param tb (shared_ptr<Table>) tables for insertion
     */
   void construct_series(Series& series,
                       const Entity& item,
                        map<string, string> target,
                        shared_ptr<Table> tb){
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
        current_id = data.at(std::stoi(target.find("row")->second)).at(col);
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
        tb->insert(series);
        }
    clean_series(series);
    }


    /**
     * @brief reads file and pushes data
     * 
     * @param path (string) directory path
     * @param delim (string) delimeter
     * @param skip_rows (int) rows to be skipped
     */
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


    /**
     * @brief Group entities for each unique key
     * 
     * @param keys (map<string, vector<Entity>> &) key and each entities 
     */
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





    /**
     * @brief Get the substring object
     * 
     * @param delimiter (string) 
     * @param extention (string) file extension
     * @param s 
     * @return std::string 
     */
    string get_substring(std::string delimiter, std::string extention, std::string s){
        std::string token;
        size_t pos = 0;
        while ((pos = s.find(delimiter)) != std::string::npos){
            token = s.substr(0, pos);
            s.erase(0, pos + delimiter.length());
        }
        pos = 0;
        while ((pos = s.find(extention)) != std::string::npos){
            token = s.substr(0, pos);
            s.erase(pos, token.length() + delimiter.length());
        }
        return s;
    }

};


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
    std::regex reg_var{item.name};
    for (size_t c = 0; c < data.at(0).size(); c++){
        lookup_id = data.at(std::stoi(cond.find("row")->second)).at(c);
        lookup_var =data.at(item.row).at(c);
        if(std::regex_match(lookup_id, reg_id) && 
            std::regex_match(lookup_var, reg_var))
            indexes.push_back(c);
        }
    }
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