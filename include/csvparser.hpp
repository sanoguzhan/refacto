#ifndef __CSVPARSER_HPP
#define __CSVPARSER_HPP

#include<iostream>
#include<iostream>
#include<string>
#include<filesystem>
#include <fstream>
#include<map>
#include "parser.hpp"


#include "boost/tokenizer.hpp"

using namespace std;
using namespace boost;
using namespace aria;
namespace fs = std::filesystem;


struct Loc{
    string name;
    string orient;
    int row;
    int column;
};
std::vector<u_int32_t> row_search(const std::vector<std::vector<string>>&,const Loc& );
class CSVParser{
    private:
        string file_path;
        std::ifstream f;
        int skip_rows = 0;
        csv::CsvParser parser;
        std::string delim = ";";


    public:
        std::vector<std::vector<string>> data;

       
        CSVParser(string path, string delim, int skip_rows): 
            file_path{path}, f(validate_f(path)),
            parser{f}, delim{delim}, 
            skip_rows{skip_rows},data{read(skip_rows)}{}

        CSVParser(string path, string delim): 
            file_path{path}, f(validate_f(path)),
            parser{f}, delim{delim},data{read(skip_rows)}{}

        CSVParser(string path, int skip_rows): 
            file_path{path}, f(validate_f(path)), 
            skip_rows{skip_rows}, parser{f}, 
            data{read(skip_rows)}{}

        CSVParser(string path): 
            file_path{path}, f(validate_f(path)), 
            parser{f}, data{read(skip_rows)}{}

    
    
    
        std::vector<string> read_line();


    private:        
        string validate_f(string);

        std::vector<std::vector<string>> read(int); 

        void move_iter(csv::Field &, int);

        inline std::vector<string> row_vector(std::string s){
            long unsigned end=0U,  start;
            std::vector<string> row_vec;
            end = s.find(delim);
            start = 0U;

            while (end != std::string::npos){
                row_vec.push_back(static_cast<string>(s.substr(start, end - start)));
                start = end + delim.length();
                    end = s.find(delim, start);
                }

            row_vec.push_back(static_cast<string>(s.substr(start, end)));
            
            return row_vec;
        }


        std::map<string, std::vector<string>> get_val(Loc& token){
            
           std::map<string, std::vector<string>> temp; 
            
   
            if(token.orient == "row"){
                row_search(data, token);
            }else if(token.orient == "column"){
                //

            }

            return temp;            
        }


        void column_search(){
            // Search item column-wise
        }

};


#endif