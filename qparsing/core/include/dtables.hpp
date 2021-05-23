
/** @file dtables.hpp
 *  @brief data table class and methods for insertion
 *
 *  This file contains the refacto's
 *  data holders for multi tables
 *
 *
 *  @todo Completation Table parsing for configuration files
 *  @todo Column-wise token search and parsing
 *  @todo type casting and arithmetic operations within rows
 *
 *  @author Oguzhan San
 *  @bug No known bugs.
 *
 */
#ifndef __DTABLES_HPP
#define __DTABLES_HPP

#include "table.hpp"
#include "yaml-cpp/yaml.h"
#include <algorithm>
using string = std::string;
using namespace table;

namespace dtable {

using Type = std::variant<string, long, double>;

namespace fs = std::filesystem;

/**
 * @brief DataRow Struct for row data container
 *  DataRow Struct
 *
 *      Data holder for Tables class
 */
struct DataRow {
  string Dtype;
  string name;
  std::vector<string> row;
};

/**
 * @brief Tables class for given configur
 *  Tables Class
 *      Creates data tables
 *      Validates configuration and data types
 */

class [[deprecated(
    "Use Table Class, configuration files will be removed")]] Tables {

public:
  string path;     // path to file
  YAML::Node file; // opened file as YML::Node
  std::vector<std::shared_ptr<DataRow>>
      data; // Data contains each column as DataRow Obj

  /**
   * @brief Saves file to running directory
   *
   * @todo take input where to save the file
   *
   */
  void save() const;

  /**
   * @brief Getter method for column
   *
   *  @param key (string) column name
   *
   * @return shared_ptr<DataRow> to column data
   */
  std::shared_ptr<DataRow> get_column(const string &) const;

  /**
   * @brief Returns Column names, dtype and size for column in data
   *
   * Prints column information of data
   *
   */
  void info() const;

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
  bool insert(std::string, std::vector<string>);
  bool insert(std::string, std::string, Series);

  /**
   * @brief Constructor
   *
   * Reads Yaml file from given path
   * Valdiates if keys exists and correct
   * Initilize DataRow for each column name and data Type
   *
   * @param path (string) Path to yaml file.
   */
  Tables(std::string);

  /**
   * @brief Returns column names of Table as vector<string>.
   *
   * Iterate over YML::Node get the first element of columns in given config
   * file
   *
   * @return Vector of column's names.
   */
  std::vector<std::string> columns();

private:
  //////////////
  bool is_column(string);
  inline size_t get_size() const {
    size_t size{0};
    for (const auto &c : data) {
      if (c->row.size() > size)
        size = c->row.size();
    }
    return --size;
  }

  //////////////

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
  void validate() const;

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
  bool compare_column(const std::string, std::string);
};

} // namespace dtable
#endif