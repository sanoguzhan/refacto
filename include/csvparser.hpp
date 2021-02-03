#ifndef __CSVPARSER_HPP
#define __CSVPARSER_HPP

#include<iostream>
#include<iostream>
#include<string>
#include<filesystem>
#include <fstream>
#include "parser.hpp"


#include<boost/tokenizer.hpp>

using namespace std;
using namespace boost;
using namespace aria;


int read_file(string path){


    std::ifstream f(path);
    csv::CsvParser parser(f);
    int i=0;
    for (;;) {
    auto field = parser.next_field();
    switch (field.type) {
      case csv::FieldType::DATA:
        if(i > 2){
            return 0;
        }
        std::cout << *field.data << " | ";
        i++;
        break;
      case csv::FieldType::ROW_END:
        std::cout << std::endl;
        break;
      case csv::FieldType::CSV_END:
        std::cout << std::endl;
        return 0;
    }
  }
    return 0;
}

#endif