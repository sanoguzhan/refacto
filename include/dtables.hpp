
/** @file dtables.hpp
 *  @brief data table class and methods for insertion
 *
 *  This file contains the refacto's
 *  data holders
 *  
 *  @author Oguzhan San
 *  @bug No known bugs.
 * 
 */ 
#ifndef __DTABLES_HPP
#define __DTABLES_HPP



#include<iostream>
#include<string>
#include<vector>
#include<variant>
#include<filesystem>
#include <utility>  
#include<memory>
#include <cctype>
#include <any>

#include "nlohmann/json.hpp"
#include "yaml-cpp/yaml.h"

using json = nlohmann::json;
using string = std::string;


namespace dtable{

    using Type = std::variant<string, long, double>;

    namespace fs = std::filesystem;

    /**
     * @brief DataRow Struct for row data container
     *  DataRow Struct 
     *      
     *      Data holder for Table class
     */
    struct DataRow{
        string Dtype;
        string name;
        std::vector<Type> row;
                 
    };

    
    /**
     * @brief Table class for given configur
     *  Table Class
     *      Creates data tables
     *      Validates configuration and data types
     */
    class Table{


        public:
            string path;
            YAML::Node file;
            std::vector<std::shared_ptr<DataRow>> data;

        


            /*COMMENT*/
            void info();
            /*COMMENT*/
            std::vector<Type> operator[](const string) const;
            // bool insert(string, std::vector<std::variant<string, long, double>>);        

            /*COMMENT*/
            template<typename T>
            bool insert(string key, std::vector<T> vec){
                for(auto column:data){            
                    if(column->name == key){
                        column->row.insert(std::end(column->row), std::begin(vec), std::end(vec));
                        return true;
                    }
                }
                return false;
            }

            /**
             * @brief Constructor
             *
             * Reads Yaml file from given path 
             * Valdiates if keys exists and correct 
             * Initilize DataRow for each column name and data Type 
             * 
             * @param path Path to yaml file.
             */
            Table(std::string);

            /**
             * @brief Returns column names of Table as vector<string>.
             *
             * Iterate over YML::Node get the first element of columns in given config file
             *
             * @return Vector of column's names.
             */
            std::vector<std::string> columns();
  

        private:

            /**
             * @brief Opens file and returns YML::Node
             *
             * Read files with Yaml 
             * Checks if file exists
             * Throws runtime_error if file not exists
             * 
             * @param path Path to yaml file.
             * 
             * @return YML::Node of opened yml file
            */
            YAML::Node open_yaml(std::string);

            /**
             * @brief Validator for column token (for constructor) 
             *
             *  Checks if Yaml has COLUMNS as a key
             *  Throw runtime_error if COLUMNS not exist in config file
             *  
            */
            void validate();



            /**
             * @brief Creates rows with DataRow structs and inserts to data
             *
             * associate column names with struct attribute 
             * Insert DataRow objects to data vector
             * 
            */
            void insert_column();

            /**
             * @brief Compares given two strings
             *
             * Creates capitilized copy of second parameter
             * Creates uppercase copy of second parameter
             * Compares copies and orginal string with first parameter
             * @param f const string, column name to be validated (from config)
             * @param s string, internal compared token
             * 
             * @return bool, true if matches with any 
            */
            bool compare_column(const std::string,std::string);

        };


}
#endif