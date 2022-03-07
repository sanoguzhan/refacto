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

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "parser.hpp"
#include "table.hpp"

using namespace std;
using namespace aria;
using namespace table;
// namespace fs = std::filesystem;
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
u_vector row_search(const std::vector<std::vector<string>> &, const Loc &, const Loc &);
u_vector row_search(const std::vector<std::vector<string>> &, const Loc &);
/**
 * @brief CSVParser Class for CSV Data Search
 *  CSVParser Class
 *      Opens file
 *      Search given data
 */
class [[deprecated("Csvparser will be removed, use CustomParser")]] CSVParser
{
private:
  string file_path;
  std::ifstream f;
  int skip_rows = 0;
  csv::CsvParser parser;
  std::string delim = ";";

public:
  /**
   * @brief resulting data of the parsing composed of 2D array with 1th axis
   * as row and 2nd as column
   *
   */
  std::vector<std::vector<string>> data;
  /**
   * @brief filename of the parsed csv file
   *
   */
  std::string file_name;
  CSVParser(string path, string delim, int skip_rows);
  CSVParser(string path, string delim);
  CSVParser(string path, int skip_rows);
  CSVParser(string path);
  /**
   * @brief Get a Serie of data
   *
   * @param orient orientation to seach the data (row or column)
   * @param idx start index to begin the search
   * @param target searching target condition
   * @param cond1 constraint target condition
   * @param cond2 second constraint target condition
   * @return Series
   */
  Series operator()(string orient, u_int32_t idx, const Loc &target, const Loc &cond1, const Loc &cond2) const;
  /**
   * @brief Get a Serie of data
   *
   * @param orient orientation to seach the data (row or column)
   * @param idx start index to begin the search
   * @param target searching target condition
   * @param cond1 constraint target condition
   * @return Series
   */
  Series operator()(string orient, u_int32_t idx, const Loc &target, const Loc &cond1) const;
  /**
   * @brief Get a Serie of data
   *
   * @param orient orientation to seach the data (row or column)
   * @param target searching target condition
   * @param idx start index to begin the search
   * @return Series
   */
  Series operator()(string orient, const Loc &target, u_int32_t idx = 0) const;
  /**
   * @brief Get a vector of range data
   *
   * @param orient orientation to seach the data (row or column)
   * @param idx index of the specified location
   * @param from start index of the other axis
   * @param to end index of the other axis
   * @return std::vector<string>
   */
  std::vector<string> operator()(std::string orient, int32_t idx, int32_t from, int32_t to) const;
  /**
   * @brief Get value of at the precised location (row + column)
   *
   * @param target location of the target (row + column)
   * @return std::string
   */
  inline std::string operator()(const Loc &target) { return data.at(target.row).at(target.column); }
  /**
   * @brief Erase multiple row or columns to clean irrelevant data
   * [first,last)
   *
   * @param orient orientation [row or column] to erase the data
   * @param start first index to erase (include)
   * @param end  last index to erase (not include)
   */
  void erase_data(string orient, int32_t start, int32_t end);
  /**
   * @brief Erase multiple rows or columns to clean pattern
   *
   * @param orient orientation [row or column] to erase the data
   * @param pattern pattern to search and erase
   */
  void erase_pattern(string orient, string pattern);
  /**
   * @brief Erase row with not the same number column as the header
   *
   */
  void erase_diverge_row();
  /**
   * @brief Save the data as it is in a csv file
   *
   * @param path filepath where to store the csv
   * @return true
   * @return false
   */
  bool to_csv(string path) const;

private:
  /**
   * @brief Check if the input csv file exist
   *
   * @return string
   */
  string validate_f(string) const;
  /**
   * @brief read a csv file and parse it to data attribute format
   *
   * @param skip number of line from the beginning to exclude of the parsing
   * @return std::vector<std::vector<string>>
   */
  std::vector<std::vector<string>> read(int skip);
  /**
   * @brief Get the substring object
   *
   * @param delimiter
   * @param extention
   * @param s
   * @return std::string
   */
  std::string get_substring(std::string delimiter, std::string extention, std::string s) const;
};
#endif