/** @file table.hpp
 *  @brief table stand-alone class for insertation parsing
 *
 *  This file contains the refacto's
 *  table class and helper methods of the class
 *
 *  @author Oguzhan San
 *  @bug No known bugs.
 *
 */
#ifndef __TABLE_HPP
#define __TABLE_HPP

// #ifndef _WIN32 || _WIN64
//     #define NAV "\"
// #else
//     #define NAV "/"
// #endif

#define NAV "/"

#include <algorithm>
#include <any>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <variant>
#include <vector>

using string = std::string;

using svector = std::vector<std::string>;

/**
 * @brief Table namespace includes Table Class and Series for table insertion
 *  Includes Table Class
 *  Includes Series Struct
 *
 */
namespace table {

/**
 * @brief Series of each Table variable, used for insertion
 *
 * Series Struct
 *
 *  Primary structure of Table construction
 *  Any Operator parsing should construct a Series
 *  Table allows any Series insertion from operators
 *
 *  name: Column Name
 *  values: key value pairs as id and each data of each id
 *
 */
struct Series {
  string name;
  std::map<string, std::vector<string>> values;
};

/**
 * @brief IDMap Class for Table insertion
 *  Used for grouping Data
 * XMLParser need this class to insert data to Table Instance
 */
struct IDMap {
  std::string name;
  std::string node;
  std::string key;
  std::string degree;
  svector values;

  IDMap(string name, string node, string key, string degree)
      : name{name}, node{node}, key{key}, degree{degree} {}
  friend bool operator>(const IDMap &right, const IDMap &left);
  friend bool operator<=(const IDMap &right, const IDMap &left);
  friend bool operator<(const IDMap &right, const IDMap &left);
  friend bool operator>=(const IDMap &right, const IDMap &left);
};

/**
 * @brief Table Class (data container)
 *  Table Class
 *      Stores Series as key value pairs
 *      Export to csv file
 */
class Table {

  std::map<std::string,
           std::vector<std::pair<std::string, std::vector<string>>>>
      data; // Data contains each column as DataRow Obj

public:
  /**
   * @brief Saves the table to given path
   *
   *  Writes table to given absolute path
   * @param path: (string) absolute path to be saved file
   *
   * @return bool: true on success
   */
  bool save(std::string) const;

  /**
   * @brief Inserts given series to table
   *
   * Copies each value of key from series to data
   *  @param series: (Series) Initilized and constructed object
   *
   * @return bool true on success
   */
  bool insert(Series);

  /**
   * @brief Inserts given value to each id
   *
   * !Data should be initilized first with a series
   * !Insertion to data without inserting any series undefined
   *
   *  @param name: (string) column name
   *  @param value: (string) value to be inserted to each ix
   *
   * @return bool true on success
   */
  bool insert(string name, string insert_name);
  bool insert(string name, string id, string insert_name);
  /**
   * @brief Inserts given vector to table
   *
   *  Copy vector value to each id
   *      !Data should be initilized first with a series
   *           !Insertion to data without inserting any series undefined
   *  @param name: (string) column name
   *  @param vector (vector<string>) values to be inserted
   *
   * @return bool true on success
   */
  bool insert(string name, std::vector<string> vec);
  bool insert(string name,string id, std::vector<string> vec);
  /**
   * @brief Getter Method of object data
   *
   * @return data
   */
  inline auto values() { return data; }

  size_t max_size() const;
  size_t get_size(string,string) const;
private:
  /**
   * @brief Checks if column exists
   *
   *  @param name: (string) column name
   *  @param target (string) token to be checked
   *
   * @return bool true if exists
   */
  bool column_exist(const string &, const string &) const;

  /**
   * @brief Getter for maximum vector size among ids
   *
   * @return size: (size_t) max. size
   */
};

inline bool operator>(const IDMap &right, const IDMap &left) {
  return right.name > left.name;
}
inline bool operator<=(const IDMap &right, const IDMap &left) {
  return right.name <= left.name;
}
inline bool operator<(const IDMap &right, const IDMap &left) {
  return right.name < left.name;
}

inline bool operator>=(const IDMap &right, const IDMap &left) {
  return right.name >= left.name;
}

} // namespace table
#endif