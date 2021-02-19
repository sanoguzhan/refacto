
/** @file table.hpp
 *  @brief table stand-alone class for insertation parsing
 *
 *  This file contains the refacto's
 *  tables
 *  
 *  @author Oguzhan San
 *  @bug No known bugs.
 * 
 */ 
#ifndef __TABLE_HPP
#define __TABLE_HPP



#include<iostream>
#include<string>
#include<vector>
#include<variant>
#include<filesystem>
#include<utility>  
#include<memory>
#include<cctype>
#include<any>
#include<typeinfo> 
#include<type_traits>
#include<fstream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using string = std::string;


namespace table{


    /**
     * @brief 
     *           
     *     
     */

    struct Series{
        string name;
        std::map<string, std::vector<string>> values;
    };

    namespace fs = std::filesystem;

    
    /**
     * @brief Table Class for data holder
     *  Table Class
     *      Stores Series as key value pairs
     *      Export to csv file
     */
    class Table{


    //     public:
    //         string path; // path to file
    //         std::vector<std::shared_ptr<Series>> data; // Data contains each column as DataRow Obj



            ////////
            void save();
            ////////


            /**
             * @brief Returns Column names, dtype and size for column in data
             *
             * Prints column information of data
             *
            */
            void info();

            /**
             * @brief Column access operator
             * 
             * Overloaded operator[]
             * Gets given column name vector
             * 
             *  @param key (string) column name
             * 
             * @return vector<T> column
             */
            std::vector<string> operator[](const string) const;

            /**
             * @brief Inserts given vector to given column name vector
             * 
             * Overloaded function
             * Inserts only when key matches with column
             * Copy each element of given vector to column
             * Overl
             *  @param key (string) column name
             *  @param vec vector<T> inserted to column
             * 
             * @return bool true on success
             */
            bool insert(std::string, std::string, Series);       


            /**
             * @brief Returns column names of Table as vector<string>.
             *
             * Iterate over YML::Node get the first element of columns in given config file
             *
             * @return Vector of column's names.
             */
            std::vector<std::string> columns();
  


        };


}
#endif