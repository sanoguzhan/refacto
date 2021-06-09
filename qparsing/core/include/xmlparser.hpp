
/** @file xmlparser.hpp
 *
 *  This file contains the refacto's xmlparser
 *      - File directory search for xml files
 *      - Recursive key search
 *      - Extractions for given IDMaps to separate csv files
 *
 *  @author Oguzhan San
 *  @bug No known bugs.
 *
 */

#ifndef XMLPARSER_HPP
#define XMLPARSER_HPP

#include <glob.h>

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <string>
#include <type_traits>
#include <utility>

#include "pugixml.hpp"
#include "table.hpp"
using namespace table;
using std::map;
using std::regex;
using std::regex_match;
using std::smatch;
using std::string;
using std::vector;
using svector = std::vector<std::string>;

static u_int32_t COUNTER = 0;
static bool CONTAINS = false;


/**
 * @brief Creates IDMaps for given IDMap which contains multi variables
 * 
 * It transform IDMap map_values,
 * For multi search operation, a IDMap stores data in the map_values attributes
 * when to_csv method is invoked, values must be converted to vector and one column for IDMap
 * This function creates IDMap for each key in map_values and inserts to IDMap vector
 * 
 * @param it: (IDMap) current id holds map_values
 * @param args: (vector<IDMap>) created ids insterted  
 */
void transform_keys(IDMap& it, vector<IDMap>& args);

/**
 * @brief Creates id value IDMap for grouping operation
 * 
 * It transform IDMap map_values,
 * For grouping operation, a IDMap stores data in the map_values attributes
 * when to_csv invoke, values must be converted to vector and one column for IDMap
 * This function uses given regex to group values as id and value
 * Creates 2 IDMap and move the data to each assicated IDMap
 * 
 * @param it: (IDMap) current id holds map_values
 * @param args: (vector<IDMap>) created ids insterted  
 */
void rotate_keys(IDMap& it, vector<IDMap>& args);

/**
 * @brief Greps the list of file for given directory
 *
 * @param dir: (string) directory with pattern match
 *      Example:
 *                  ./*.xml
 *
 * @return vector<string> as vector of matched files
 */


vector<string> listdir(string);

/**
 * @brief XMLParser Class for XML file search and value transform
 *  Keeps arbitary number of IDMaps and construct Vector with Initializer List
 */
class XMLParser {
   public:
    std::vector<IDMap> data;

    /**
     * @brief XMLParser Constructer
     *      - Takes arbitary number of IDMaps
     *      - Construct a Vector and classify with the name of IDMap
     *
     * @param lst List IDMap
     */
    template <typename... T>
    XMLParser(T... lst) : data{lst...} {}

    /**
     * @brief Writes data to csv files
     *      - Writes each key variable to diffirent
     *      - appends the file names to given directory path
     *
     * @param dir: (string) extraction directory
     * @return true on success
     */
    bool to_csv(string dir);

    std::string inline write_header(std::pair<const string, vector<IDMap>> &p);

    /**
     * @brief Operator for xml file data search
     *         - Takes directory as input and looks for pattern match
     *         - Search for variables for each IDMap
     *
     * @param path: (string) path to xml file directory
     * @param root_name : (string) root name in the xml files
     */
    void operator()(string path, const string root_name);

   private:
    /**
     * @brief Read for operator()
     *      - Reader for xml files
     * @param path : (string) path to file
     * @param root_name : (string) xml root name
     * @param doc : (pugi::xml_document) opened document
     * @return decltype(auto)
     */
    inline decltype(auto) read(string path, const string root_name,
                               pugi::xml_document &doc);

    /**
     * @brief Update ids vector from xml tree
     *      - Recursive function to tree-traversal
     *
     * @param root : (pugi::xml_node) root node
     * @param tag : (IDMap) search IDMap
     * @param ids : (vector<string>) id vector which filled up
     */
    void update(pugi::xml_node root, const IDMap &tag, svector &ids);
    void update(pugi::xml_node root, const IDMap &tag,
                map<string, vector<string>> &ids);
    /**
     * @brief Map trasnformers to name-value map
     *      - Create map from IDMap for each unique name
     *      - Classify IDMap vector to unique name and values
     * @param keys key-value pairs
     */
    void transfrom_map(map<string, vector<IDMap>> &keys);

    /**
     * @brief Create variable size map for writing
     *
     * @param keys key-value map for each unique IDMap
     * @param sizes size key-value map for name+key values
     */
    void inline max_key_sizes(map<string, vector<IDMap>> &keys,
                              map<string, u_int32_t> &sizes);
};

#endif
