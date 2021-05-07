#include"xmlparser.hpp"


static string DELIMETER = ";";

vector<string> listdir(string pattern){
    pattern += "*.xml";
    glob_t glob_result;
    glob(pattern.c_str(),GLOB_TILDE,NULL,&glob_result);
    vector<string> files;
    for(unsigned int i=0;i<glob_result.gl_pathc;++i){
        files.push_back(string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return files;
}
void  XMLParser::max_key_sizes(map<string, vector<IDMap>>& keys,
                            map<string, u_int32_t>& sizes){
            u_int64_t size = 0;
            for(auto& p:keys){
                for(const auto& id:p.second){
                    sizes.insert(std::pair<string, u_int32_t>(id.name + id.key, id.values.size()));
                }
            }
        }

decltype(auto) XMLParser::read(string path,
                                  const string root_name,
                                  pugi::xml_document &doc){
            if (!doc.load_file(path.c_str()))
                throw std::runtime_error("File not exist at " +  path);
            pugi::xml_node _node = doc.child(root_name.c_str());
            if(!_node)
                throw std::runtime_error("Root name is not correct " +  root_name);
            return std::move(_node);
        }
void XMLParser::transfrom_map(map<string, vector<IDMap>>& keys){
    vector<string> names;
    std::sort(data.begin(), data.end());
    std::transform(data.begin(), data.end(), back_inserter(names), [](IDMap &id){
        return id.name;
    });
    names.erase(unique(names.begin(), names.end()), names.end());
    transform(names.begin(), names.end(), std::inserter(keys, keys.end()), [](string &key){
        return std::make_pair(key, vector<IDMap>());
    });
    for_each(data.begin(), data.end(), [&](IDMap& id){
        keys.at(id.name).emplace_back(id);
    });
}

bool XMLParser::to_csv(string dir){

    map<string, vector<IDMap>> keys;
    map<string, u_int32_t> key_sizes, level_sizes;
    transfrom_map(keys);

    size_t i, row = 0, col = 0;
    bool header = true;
    max_key_sizes(keys, key_sizes);

    std::stringstream ss;

    for(auto& key:keys){
        level_sizes.insert(std::pair<std::string, u_int32_t>(key.first, 
        std::max_element(key.second.begin(),key.second.end(),[] (const IDMap& a,const IDMap& b)->bool{ 
                return a.values.size() < b.values.size();})->values.size()));
    }
    for(auto& p:keys){

        std::ofstream ofs(dir + "/" + p.first + ".csv", std::ofstream::out);
            for(i = 0;i < p.second.size()-1; ++i){
                ss << p.second.at(i).key << DELIMETER;
            }
            ss << p.second.at(i).key << "\n";
        for(row=0;row < level_sizes.at(p.first); row++ ){
            for(col=0;col < p.second.size()-1; col++){
                if( row  < key_sizes.at(p.second.at(col).name + p.second.at(col).key)){
                    ss << p.second.at(col).values.at(row) << DELIMETER;
                }else{ss << DELIMETER;}
            }
            if(row  < key_sizes.at(p.second.at(col).name + p.second.at(col).key)){
                ss << p.second.at(col).values.at(row) << "\n";
            }else{ss <<"\n";}
        }
        ofs << ss.str();
        ofs.close();
        ss.str(std::string());
    }
    return true;
}


void XMLParser::update(pugi::xml_node root,
                    const IDMap &tag,
                    svector &ids){

    root = (!COUNTER) ? root.first_child(): root;
    COUNTER++;
    string token, id;
    for (pugi::xml_node panel = root.first_child(); panel; panel = panel.next_sibling()){
        token = panel.name();
            if(token == tag.node && string(panel.child_value()).find(tag.key) != string::npos){
                CONTAINS = true;
                id = panel.child_value();
            }
            if(CONTAINS){
                if(token.find(tag.degree) != string::npos){
                    ids.push_back(panel.child_value());
                }
            }
        }
    CONTAINS = false;
    root = root.next_sibling();
    if(root) update(root, tag, ids);
}


void XMLParser::operator()(string path, const string root_name){
    for(const auto& file: listdir(path)){
        pugi::xml_document doc;
        pugi::xml_node root{read(file, root_name, doc)};
        svector ids;
        for(auto& tag: data){
            update(root, tag, ids);
            tag.values.insert(tag.values.end(), ids.begin(), ids.end());
            ids.clear();
            COUNTER = 0;
        }
    }
}
