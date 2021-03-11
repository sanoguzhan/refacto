/** @file csvparser.hpp
 *  @brief CSVParser operator for data accessing and structring
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

#ifndef __CSVPARSER_HPP
#define __CSVPARSER_HPP

#include <iostream>
#include <iostream>
#include <string>
#include <filesystem>
#include <regex>
#include <fstream>
#include <map>
#include <utility>
#include <vector>
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
namespace fs = std::filesystem;

using u_vector = const std::vector<u_int32_t>;

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
struct Loc
{
    string name;
    string orient;
    int row;
    int column;
};

/**
 * @brief Search Methods (Internal)
 * 
 * Searches given Loc constions on the csv file
 * 
 *  Takes one or two loc object as condition
 *  
 *  @param vector: (vector<vector<string&>>) opened csv file data
 *  @param loc: initilized and constructed Loc objects
 * 
 * @return found indexes
 */
u_vector row_search(const std::vector<std::vector<string>> &,
                    const Loc &, const Loc &);
u_vector row_search(const std::vector<std::vector<string>> &,
                    const Loc &);

/**
 * @brief CSVParser Class for CSV Data Search
 *  CSVParser Class
 *      Opens file
 *      Search given data
 */
class CSVParser
{
private:
    string file_path;
    std::ifstream f;
    int skip_rows = 0;
    csv::CsvParser parser;
    std::string delim = ";";

public:
    std::vector<std::vector<string>> data;
    std::string file_name;

    CSVParser(string path, string delim, int skip_rows);

    CSVParser(string path, string delim);

    CSVParser(string path, int skip_rows);

    CSVParser(string path);

    std::vector<string> read_line();

    // !Comment
    Series values(string orient,
                  u_int32_t idx,
                  const Loc &target,
                  const Loc &cond1,
                  const Loc &cond2);

    // !Comment
    Series values(string orient,
                  u_int32_t idx,
                  const Loc &target,
                  const Loc &cond1);

    // !Comment
    Series values(string orient,
                  const Loc &target,
                  u_int32_t idx = 0);

    // !Comment
    std::vector<string> values(std::string orient,
                               int32_t idx,
                               int32_t from,
                               int32_t to);

    // !Comment
    inline std::string value(const Loc &target)
    {
        return data.at(target.row).at(target.column);
    }

    // Erase multiple row or columns to clean irrelevant data [first,last)
    void erase_data(string orient, int32_t start, int32_t end);

    // Erase multiple rows or columns to clean pattern
    void erase_pattern(string orient, string pattern);

    // Erase row with not the same number column as the header
    void erase_diverge_row();

    // Save the data as it is in a csv file
    bool save_value_in_file(fs::path path);

private:
    string validate_f(string);

    std::vector<std::vector<string>> read(int);
    std::string get_substring(std::string, std::string, std::string);
    void move_iter(csv::Field &, int);

    inline std::vector<string> row_vector(std::string s)
    {
        long unsigned end = 0U, start;
        std::vector<string> row_vec;
        end = s.find(delim);
        start = 0U;

        while (end != std::string::npos)
        {
            row_vec.push_back(static_cast<string>(s.substr(start, end - start)));
            // std::cout << static_cast<string>(s.substr(start, end - start)) << std::endl;
            start = end + delim.length();
            end = s.find(delim, start);
        }

        row_vec.push_back(static_cast<string>(s.substr(start, end)));

        return row_vec;
    }

    void column_search()
    {
        // Search item column-wise
    }
};

#endif