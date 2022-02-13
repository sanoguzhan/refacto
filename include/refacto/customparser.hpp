/** @file csvparser.hpp
 *  @brief Custom Parser csv operator for data accessing and structring
 *
 *  This file contains the refacto's customparser
 *  includes id search and variable insertion
 *
 *
 *  @author Oguzhan San
 *  @bug No known bugs.
 *
 */

#ifndef __CUSTOMPARSER_HPP
#define __CUSTOMPARSER_HPP

#include <glob.h>
#include <sys/stat.h>

#include <algorithm>
#include <boost/log/trivial.hpp>
#include <cctype>
#include <deque>
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

#define NAV "/"

using namespace std;
using namespace aria;
using namespace table;

using u_vector = const std::vector<u_int32_t>;
using s_matrix = const std::vector<std::vector<string>>;

using std::deque;
using std::map;
using std::shared_ptr;
using std::string;
using std::vector;

/**
 * @brief Entity struct for paramaterization of variables
 *
 * Entity Struct
 *
 *  Variable search object, used to locate variables
 *
 * Used for
 *  - ID Search-create
 *  - Variable seach
 *  - Vector Search
 *  - Single value search
 *
 */

struct Entity {
    string key;  // meta info ( example: inverter)
    string name;
    string orient;
    string eType;
    int value_begin;
    vector<map<string, string>> conditions;

    int row;
    int column;
    int vFrom;
    int vTo;

    string keyword;  // multi or filename

    // For single value insertion (entity)
    Entity(string key, string name, string eType, string keyword)
        : key{key}, name{name}, eType{eType}, keyword{keyword} {}

    // For Vector Insertation
    Entity(string key, string name, string orient, string eType, string keyword,
           int column, int vFrom, int vTo)
        : key{key},
          name{name},
          orient{orient},
          eType{eType},
          keyword{keyword},
          column{column},
          vFrom{vFrom},
          vTo{vTo} {}

    // For id initilization
    Entity(string key, string name, string orient, string eType, int row,
           int column, int value_begin, string keyword = "multi")
        : key{key},
          name{name},
          orient{orient},
          eType{eType},
          row{row},
          column{column},
          value_begin{value_begin},
          keyword{keyword} {}

    // Series initilization
    Entity(string key, string name, string orient, string eType, int row,
           int value_begin = 0)
        : key{key},
          name{name},
          orient{orient},
          eType{eType},
          row{row},
          value_begin{value_begin} {}
    // Series initilization with condition
    Entity(string key, string name, string orient, string eType,
           vector<map<string, string>> conditions, int value_begin = 0)
        : key{key},
          name{name},
          orient{orient},
          eType{eType},
          value_begin{value_begin},
          conditions{conditions} {}

    friend bool operator>(const Entity &right, const Entity &left);
    friend bool operator<=(const Entity &right, const Entity &left);
    friend bool operator<(const Entity &right, const Entity &left);
    friend bool operator>=(const Entity &right, const Entity &left);
};

inline bool is_path_exist(const std::string &s) {
    struct stat buffer;
    return (stat(s.c_str(), &buffer) == 0);
}

void find_ids(s_matrix &, vector<u_int32_t> &, const Entity &);
void find_ids(s_matrix &, vector<u_int32_t> &, map<string, string>,
              const Entity &);

/**
 * @brief Get the flist object
 *
 * @param pattern "file extension"
 * @return deque<string>
 */
deque<string> get_flist(string pattern);

/**
 * @brief CustomParser Class for CSV Data Search
 *  CSVParser Class
 *      Opens file
 *      Search given data
 */
class CustomParser {
   private:
    vector<Entity> meta;

   public:
    /**
     * @brief resulting data of the parsing composed of 2D array with 1th axis
     * as row and 2nd as column
     *
     */
    std::vector<std::vector<string>> data;
    std::map<string, shared_ptr<Table>> tables;

    template <typename... T>
    CustomParser(T... lst) : meta{lst...} {}

   private:
    string id_iter, id_type;
    vector<string> id_iter_vector;
    std::string file_name;

   public:
    /**
     * @brief Overload ()operator for hyperparameters
     *
     * @param dir_path
     * @param delimeter
     * @param skip
     */
    void operator()(string dir_path, string delimeter, int skip);

    /**
     * @brief Insert series to tables
     *
     *  Parse values to series
     *  insert series to table
     *  flush series for each entity
     *
     *
     * @param tb (shared_ptr<Table>)
     * @param entity_lst (const vector<Entity> &) conditional search
     * @param series (Series &) series to be inserted
     * @param vec_values (vector<string> &) vector values to be inserted
     */
    void in_insert(shared_ptr<Table> tb, const vector<Entity> &entity_lst,
                   Series &series, vector<string> &vec_values);

    /**
     * @brief Insert values to vector
     *
     *  Parse values vec_values
     * @param item ( Entity&) conditional search
     * @param vec_values (vector<string> &) vector values to be inserted
     */
    void from_vector(vector<string> &vec_values, Entity &item) const;

    /**
     * @brief Get the variable name object
     *  - Search key name with regex
     * @return (string) found id
     */
    string get_variable_name(const Entity &) const;

    /**
     * @brief Insert group ids with regex search
     *  - Custom search for id as vairable
     *
     * @param series (Series &) series to be inserted
     * @param item ( Entity&) conditional search
     * @param tb (shared_pr<Table>) table which data will be inserted
     */
    void from_group_series(Series &series, const Entity &item,
                           shared_ptr<Table> tb);

    /**
     * @brief writes to csv
     *
     * @param dir (string) directory path
     */
    inline void to_csv(string dir) {
        string save_path;
        for (const auto &p : tables) {
            save_path = dir + "/" + p.first + ".csv";
            p.second->save(save_path);
        }
    }

   private:
    /**
     * @brief Creates series with entity condition
     *
     * @param series (Series) series which will be filled
     * @param item (Entity) condition for series construction
     * @param tb (shared_pr<Table>) table which data will be inserted
     */
    void from_series(Series &series, const Entity &item, shared_ptr<Table> tb);

    /**
     * @brief Create a ids and initilize map keys
     *
     * @param series (Series&) series for id initilization
     * @param item (Entity) id search condition
     */
    void create_ids(Series &series, const Entity &item);
    /**
     * @brief Checks if id exists
     *
     * @param series (Series&) Series id will be checked
     * @param check_id (string) id to be checked
     * @return true if exists
     * @return false if not
     */
    inline bool id_exist(Series &series, string check_id) {
        if (series.values.find(check_id) == series.values.end()) return false;
        return true;
    }

    /**
     * @brief Construct series for insertion
     *
     * @param series (Series&) series for construction
     * @param item (Entity&) Search conditions
     * @param tb (shared_ptr<Table>) tables for insertion
     */
    void construct_series(Series &series, const Entity &item,
                          shared_ptr<Table> tb);

    void inline clean_series(Series &series) const {
        for (auto &p : series.values) {
            series.values.find(p.first)->second.clear();
        }
    }

    /**
     * @brief Construct series for insertion with condition
     *
     *  - Conditional id search
     *
     * @param series (Series&) series for construction
     * @param item (Entity&) Search conditions
     * @param target (map<string, string>) condition
     * @param tb (shared_ptr<Table>) tables for insertion
     */
    void construct_series(Series &series, const Entity &item,
                          map<string, string> target,
                          shared_ptr<Table> tb) const;

    /**
     * @brief reads file and pushes data
     *
     * @param path (string) directory path
     * @param delim (string) delimeter
     * @param skip_rows (int) rows to be skipped
     */
    inline void in_read(const string &path, string delim, int skip_rows) {
        std::ifstream file(path);
        csv::CsvParser p{file};

        p.delimiter(delim[0]);
        int to_skip = 0;

        for (auto iter_row = p.begin(); iter_row != p.end(); iter_row++) {
            if (to_skip < skip_rows) {
                to_skip++;
                continue;
            }
            data.push_back(*iter_row);
        }
    }

    /**
     * @brief Group entities for each unique key
     *
     * @param keys (map<string, vector<Entity>> &) key and each entities
     */
    void transform_keys(map<string, vector<Entity>> &keys);

    /**
     * @brief Get the substring object
     *
     * @param delimiter (string)
     * @param extention (string) file extension
     * @param s
     * @return std::string
     */
    string get_substring(std::string delimiter, std::string extention,
                         std::string s) const;
};

inline bool operator>(const Entity &right, const Entity &left) {
    return right.key > left.key;
}
inline bool operator<=(const Entity &right, const Entity &left) {
    return right.key <= left.key;
}
inline bool operator<(const Entity &right, const Entity &left) {
    return right.key < left.key;
}

inline bool operator>=(const Entity &right, const Entity &left) {
    return right.key >= left.key;
}

#endif