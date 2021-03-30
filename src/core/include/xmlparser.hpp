
/** @file csvparser.hpp
 *  @brief
 *
 *  This file contains the refacto's xmlparser
 *
 *
 *
 *  @author Oguzhan San
 *  @bug No known bugs.
 *
 */

#ifndef __XMLPARSER_HPP
#define __XMLPARSER_HPP

#include<iostream>
#include<iostream>
#include<string>
#include<filesystem>
#include<fstream>
#include<map>
#include<utility>
#include<cstring>
#include<initializer_list>

#include "table.hpp"
#include "pugixml.hpp"

using namespace table;
namespace fs = std::filesystem;


static u_int32_t COUNTER = 0;
static bool CONTAINS = false;

void reinit(svector &);




class XMLParser{

    private:


    public:
        std::vector<IDMap> data;

        template<typename ...T>
        XMLParser(T ...lst): data{lst...}{}



        inline decltype(auto) read(string path,
                                  const string root_name,
                                  pugi::xml_document &doc){

            if (!doc.load_file(path.c_str()))
                throw std::runtime_error("File not exist at " +  path);
            pugi::xml_node _node = doc.child(root_name.c_str());
            if(!_node)
                throw std::runtime_error("Root name is not correct " +  root_name);

            return std::move(_node);
            }


        void operator()(std::string path, const std::string root_name){
            pugi::xml_document doc;
            pugi::xml_node root{read(path, root_name, doc)};
            svector ids;
            for(auto& tag: data){
                update(root, tag, ids);
                tag.values.insert(tag.values.end(), ids.begin(), ids.end());
                reinit(ids);
            }
        }



        inline void update(pugi::xml_node root,
                            const IDMap &tag,
                            svector &ids){

            root = (!COUNTER) ? root.first_child(): root;
            COUNTER++;
            std::string token, id;
            for (pugi::xml_node panel = root.first_child(); panel; panel = panel.next_sibling()){
                token = panel.name();
                    if(token == tag.node && std::string(panel.child_value()).find(tag.key) != std::string::npos){
                        CONTAINS = true;
                        id = panel.child_value();
                    }
                    if(CONTAINS){
                        if(token.find(tag.degree) != std::string::npos){
                            ids.push_back(panel.child_value());
                        }
                    }
                }
            CONTAINS = false;
            root = root.next_sibling();
            if(root) update(root, tag, ids);
        }

        bool to_csv(){

            std::map<std::string, std::vector<IDMap>> keys;
            reconstruct(keys);
            size_t i, row = 0, col = 0;
            bool header = true;
            for(auto& p:keys){
                std::cout << p.first << std::endl;
                    for(i = 0;i < p.second.size()-1; ++i){
                        std::cout << p.second.at(i).key << ";";
                    }
                     std::cout << p.second.at(i).key << std::endl;

                for(row=0;row < max_size(p.second)-1; ++row ){
                    for(col=0;col < p.second.size()-1; col++){
                        std::cout << p.second.at(col).values.at(row) << ";";
                    }
                    std::cout << p.second.at(col).values.at(row)  << std::endl;
                }
            }
            return true;
        }

        u_int64_t max_size(const std::vector<IDMap> &ids){
            u_int64_t size = 0;
            for(const auto& id:ids){
                size = id.values.size() >= size ? id.values.size() : 0;
            }
            return size;
        }


        void reconstruct(std::map<std::string, std::vector<IDMap>>& keys){
            std::vector<std::string> names;
            std::sort(data.begin(), data.end());
            std::transform(data.begin(), data.end(), back_inserter(names), [](IDMap &id){
                return id.name;
            });
            names.erase(unique(names.begin(), names.end()), names.end());
            transform(names.begin(), names.end(), std::inserter(keys, keys.end()), [](string &key){
                return std::make_pair(key, std::vector<IDMap>());
            });
            for_each(data.begin(), data.end(), [&](IDMap& id){
                keys.at(id.name).emplace_back(id);
            });
        }
};



inline void reinit(svector &ids){
    ids.clear();
    COUNTER = 0;
}

#endif