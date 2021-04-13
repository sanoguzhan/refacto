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
#include<iostream>
#include"csvparser.hpp"
#include"table.hpp"
#include"xmlparser.hpp"

using namespace std;

using dict =  map<string, map<string,string>>;
using csv_args = map<string,vector<map<string,map<string,string>>>>;

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


// Reason for this class 
//  Cython is buggy when it comes to overloaded functions and memory management
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