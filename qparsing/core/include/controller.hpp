/** @file controller.hpp
 *  @brief Contains each parser with wrapper for Cython extension
 *
 *  This file contains the refacto's each parser wrapper
 *  Header only for extending from cython
 *  
 *  Acst as Proxy between refacto c++ library and parsing python library
 *  @author Oguzhan San
 *  @bug No known bugs.
 * 
 */
#ifndef __CONTROLLER_HPP
#define __CONTROLLER_HPP

#include<iostream>
#include<string>
#include<memory>
#include<variant>
#include<map>
#include<utility>
#include<iostream>
#include"csvparser.hpp"
#include"table.hpp"
#include"xmlparser.hpp"
#include"customparser.hpp"
using namespace std;

using dict =  map<string, map<string,string>>;
using csv_args = map<string,vector<map<string,map<string,string>>>>;



class CustomParserWrapper{
    vector<map<string, string>> kwargs;
    vector<Entity> vargs;
    CustomParser parser;
    public:
    CustomParserWrapper(vector<map<string, string>> kwargs ,map<string,map<string, string>> condition)
    :  parser{validate(kwargs, condition)}{}   


   vector<Entity> validate(vector<map<string, string>> values,
                 map<string, map<string, string>> condition){
        vector<Entity> entity;

        for(auto i:values){
            map<string, string>::iterator condition_name {i.find("condition")};
            if(i.at("type") == "series"
             || i.at("type") == "group") {
                if(condition_name == i.end())
                    init_series_pyx(i, entity);
                else{
                    if(i.at("type") == "series") 
                        init_series_pyx(i, entity, condition.at(i.at("key") + i.at("name")));
                    else
                        init_series_group_pyx(i, entity, condition.at(i.at("key") + i.at("name")));
                }    
            }else if(i.at("type") == "ids"){
                    init_series_ids_pyx(i, entity);
            }else if(i.at("type") == "vector"){
                    init_vector_ids_pyx(i, entity);
            }else if(i.at("type") == "entity"){
                    init_series_entity_pyx(i, entity);
            }else{
                throw std::runtime_error("Keys not found: [series, ids, vector]");
            }
        }
        return entity;
    }

    void save(string dir){
        parser.to_csv(dir);
    }

    void operator()(string dir_path, string delimeter, int skip){
            parser(dir_path, delimeter, skip);
    }

    void init_series_entity_pyx(map<string, string>& vals,
                            vector<Entity>& entity){
                            
            entity.emplace_back(vals.at("key"),
                            vals.at("name"),
                            vals.at("type"),
                            vals.at("keyword"));    
    }

    void init_vector_ids_pyx(map<string, string>& vals,
                            vector<Entity>& entity){

            entity.emplace_back(vals.at("key"),
                            vals.at("name"),
                            vals.at("orient"),
                            vals.at("type"),
                            "vector",
                            std::stoi(vals.at("column")),
                            std::stoi(vals.at("from")),
                            std::stoi(vals.at("to")));
    }   

    void init_series_ids_pyx(map<string, string>& vals,
                            vector<Entity>& entity){
        if(vals.at("keyword") == "file_name"){
            entity.emplace_back(vals.at("key"),
                            vals.at("name"),
                            vals.at("type"),
                            vals.at("keyword"));
        }else if(vals.at("keyword") =="multi"){
            entity.emplace_back(vals.at("key"),
                            vals.at("name"),
                            vals.at("orient"),
                            vals.at("type"),
                            std::stoi(vals.at("row")),
                            std::stoi(vals.at("column")),
                            std::stoi(vals.at("value_begin")),
                            vals.at("keyword"));
        }else if(vals.at("keyword") == "single"){
            entity.emplace_back(vals.at("key"),
                        vals.at("name"),
                        vals.at("orient"),
                        vals.at("type"),
                        std::stoi(vals.at("row")),
                        std::stoi(vals.at("column")),
                        std::stoi(vals.at("value_begin")),
                        vals.at("keyword"));
        }else{
            throw std::runtime_error("Wrong configuration");
        }
    }

    void init_series_pyx(map<string, string>& vals, 
                    vector<Entity>& entity,
                    map<string, string>& condition){
        vector<map<string,string>> cond;
        cond.push_back({{"name",condition.at("name")},
                        {"orient", condition.at("orient")},
                        {"row", condition.at("row")}});


        check_keys_pyx(vals);
        entity.emplace_back(vals.at("key"),
                            vals.at("name"),
                            vals.at("orient"),
                            vals.at("type"),
                            std::stoi(vals.at("row")),
                            std::stoi(vals.at("value_begin")));

        entity.back().conditions = cond;
        }

    void init_series_group_pyx(map<string, string>& vals, 
                    vector<Entity>& entity,
                    map<string, string>& condition){
        vector<map<string,string>> cond;
        cond.push_back({{"id",condition.at("id")},
                        {"name", condition.at("name")}});


        check_keys_pyx(vals);
        entity.emplace_back(vals.at("key"),
                            vals.at("name"),
                            vals.at("orient"),
                            vals.at("type"),
                            std::stoi(vals.at("row")),
                            std::stoi(vals.at("value_begin")));

        entity.back().conditions = cond;
        }
        
         
    void init_series_pyx(map<string, string>& vals, vector<Entity>& entity){
       
        check_keys_pyx(vals);
        entity.emplace_back(vals.at("key"),
                            vals.at("name"),
                            vals.at("orient"),
                            vals.at("type"),
                            std::stoi(vals.at("row")),
                            std::stoi(vals.at("value_begin")));
    }


    void check_keys_pyx(map<string, string> &vals){
        if(not_exists(vals, "key") ||
            not_exists(vals, "name") ||
            not_exists(vals, "orient") ||
            not_exists(vals, "type") ||
            not_exists(vals, "row") ||
            not_exists(vals, "value_begin"))
                throw std::runtime_error("Series keys are not matched!");
    }


    bool not_exists(map<string, string>& vals, string key){
        std::map<string, string>::iterator it{vals.find(key)};
        if(it == vals.end())
            return true;
        return false;
    }
};

class CSVParserWrapper{
    /**
     * @brief CSVParser Wrapper with Table class
     *  Table csvparser run this wrapper together to aviod returning any values to Python
     */
    public:

        shared_ptr<CSVParser> csvparser;
        shared_ptr<Table> table;
        
    CSVParserWrapper(dict kwargs)
            :table{make_shared<Table>()}{
        if(kwargs.find("csvparser") != kwargs.end()){
            init_csvparser(kwargs.at("csvparser"));
        }
    }

    void inline init_csvparser(map<string,string>& vars){
            csvparser.reset();
            csvparser = make_shared<CSVParser>(vars.at("path"),
                                            vars.at("delim"),
                                            stoi(vars.at("skip")));       
    }

    void inline from_csv0(string orient,
                  const Loc& target,
                  u_int32_t idx){
            table->insert((*csvparser)(orient, target, idx));
        }

    void inline from_csv2(string orient,
                  u_int32_t idx,
                  const Loc &target,
                  const Loc &cond1,
                  const Loc &cond2){
        table->insert((*csvparser)(orient, idx, cond1, cond2));
    }

    void inline from_csv1(string orient,
                  u_int32_t idx,
                  const Loc &target,
                  const Loc &cond1){
    table->insert((*csvparser)(orient, idx, target, cond1));
    }

    void inline from_csv_vec(string column,
                         string orient,
                        int32_t idx,
                        int32_t from,
                        int32_t to){
        table->insert(column, (*csvparser)(orient, idx, from, to));
    }
    void inline from_csv(string column ,
                        const Loc& target){
        table->insert(column, (*csvparser)(target));
    }

    void inline from_csv_static(string column, string value){
        table->insert(column, value);
    }

    void inline to_csv(string path){
        table->save(path);
    }

};

inline vector<IDMap> clean_values(vector<map<string, string>> kwargs){
         vector<IDMap> values;
         for(const auto& p:kwargs){
             values.emplace_back(p.at("name"),
                                p.at("node"),
                                p.at("key"),
                                p.at("degree"));
            }
         return values;
     }


//  XMLParser expects initilizer list however it is not possible with Cython
//  That is why we take all parameters of IDMap as vector of maps and construct in cpp
//  if we do other way around, it makes buggy code to create this vector of IDMap 
//  then we need to be careful not create python objects on the cython part etc.
//  This solution allows as to create a layer of abstraction where we can also validate input
class XMLParserController :public XMLParser {
    public:
        XMLParserController(vector<map<string, string>>lst):
        XMLParser{clean_values(lst)}{}

    void get(string dir, const string root){
        XMLParser::operator()(dir,root);
    }
    
    
};
#endif