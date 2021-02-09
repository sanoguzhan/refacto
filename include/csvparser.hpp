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

    
    
        string validate_f(string);
    
        std::vector<string> read_line(){
            long unsigned end, start = 0U;
            std::vector<string> vec;
            for(;;){
                auto field = parser.next_field();
                string s;
                if(field.type == csv::FieldType::ROW_END
                    || field.type == csv::FieldType::CSV_END) continue;
                
                s = *field.data;                
                end = s.find(delim);

                while (end != std::string::npos){
                    vec.push_back(static_cast<string>(s.substr(start, end - start)));
                    start = end + delim.length();
                    end = s.find(delim, start);
                }
                vec.push_back(static_cast<string>(s.substr(start, end)));
                break;
            }
            return vec;
        }    


        
        std::vector<std::vector<string>> read(int skip){
            std::vector<std::vector<string>> vec;
            auto field = parser.next_field();
            
            move_iter(field, skip);
            
            while(field.type != csv::FieldType::CSV_END){
                if(field.type == csv::FieldType::ROW_END){
                    field = parser.next_field();
                    continue;
                }
                vec.push_back(row_vector(static_cast<string>(*field.data)));
                field = parser.next_field();
            }
            return vec;
        }    


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
                row_search(token);
            }else if(token.orient == "column"){
                //

            }

            return temp;            
        }

        void row_search(Loc& token){
            string lookup;
            for(size_t r=0; r < data.size(); r++){
                lookup = data.at(token.row).at(r);
                if(lookup.find(token.name) != string::npos)
                    std::cout << lookup << "\n";
                }
        }

        void column_search(){
            // Search item column-wise
        }

        void move_iter(csv::Field &field, int skip){
            int counter = 0;
             while(counter < skip-1){
                    if(field.type == csv::FieldType::ROW_END){
                        counter++;
                        field = parser.next_field();
                        continue;
                    } 
                    field = parser.next_field();
                    continue;
                }
    }

};


#endif