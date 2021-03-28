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


using namespace std;

using dict =  map<string, map<string,string>>;
using csv_args = map<string,vector<map<string,map<string,string>>>>;

class CSVParserWrapper{
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

    void inline to_csv(string path){
        table->save(path);
    }

};

#endif