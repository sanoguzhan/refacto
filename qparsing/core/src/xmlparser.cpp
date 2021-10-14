#include "xmlparser.hpp"

static string DELIMETER = ";";
vector<string> listdir(string pattern) {
    if (pattern.compare(pattern.size() - 1, 1, "/") != 0) pattern += "/";
    string cwdir = pattern;
    pattern += "*.xml";
    glob_t glob_result;
    glob(pattern.c_str(), GLOB_TILDE, NULL, &glob_result);
    vector<string> files;
    for (unsigned int i = 0; i < glob_result.gl_pathc; ++i) {
        files.push_back(string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    if (files.empty()) throw std::runtime_error("Files Not Found at " + cwdir);
    return files;
}

void XMLParser::max_key_sizes(map<string, vector<IDMap>>& keys,
                              map<string, u_int32_t>& sizes) {
    for (auto& p : keys) {
        for (const auto& id : p.second) {
            sizes.insert(std::pair<string, u_int32_t>(id.name + id.key,
                                                      id.values.size()));
        }
    }
}

decltype(auto) XMLParser::read(string path, const string root_name,
                               pugi::xml_document& doc) {
    if (!doc.load_file(path.c_str()))
        throw std::runtime_error("File not exist at " + path);
    pugi::xml_node _node = doc.child(root_name.c_str());
    if (!_node)
        throw std::runtime_error("Root name is not correct " + root_name);
    return _node;
}

void XMLParser::transfrom_map(map<string, vector<IDMap>>& keys) {
    vector<string> names;
    std::sort(data.begin(), data.end());
    std::transform(data.begin(), data.end(), back_inserter(names),
                   [](const IDMap& id) { return id.name; });
    names.erase(unique(names.begin(), names.end()), names.end());
    transform(names.begin(), names.end(), std::inserter(keys, keys.end()),
              [](string& key) { return std::make_pair(key, vector<IDMap>()); });
    for_each(data.begin(), data.end(),
             [&](IDMap& id) { keys.at(id.name).emplace_back(id); });
}

void transform_keys(IDMap& it, vector<IDMap>& args) {
    const vector<IDMap> columns = [&it]() {
        vector<IDMap> ids;
        std::transform(
            it.map_values.begin(), it.map_values.end(), std::back_inserter(ids),
            [&](auto item) -> IDMap {
                return {it.name, it.node, item.first, it.degree, item.second};
            });
        return ids;
    }();
    it.key = "NULL";
    args.insert(args.begin(), columns.begin(), columns.end());
}

void rotate_keys(IDMap& it, vector<IDMap>& args) {
    svector ids, vars;
    if (it.conditions.find("id") == it.conditions.end() ||
        it.conditions.find("name") == it.conditions.end())
        throw std::runtime_error("Conditions missing 'id' or 'name' keys.");
    const std::regex e_id(it.conditions.at("id"));
    const std::regex e_var(it.conditions.at("name"));

    std::smatch sm_id, sm_var;
    std::for_each(
        it.map_values.begin(), it.map_values.end(), [&](const auto p) {
            std::regex_match(p.first, sm_id, e_id,
                             std::regex_constants::match_default);
            std::regex_match(p.first, sm_var, e_var,
                             std::regex_constants::match_default);
            if (sm_id.size() > 0 && sm_var.size() > 0) {
                ids.insert(ids.end(), p.second.size(), sm_id[1]);
                vars.insert(vars.end(), p.second.begin(), p.second.end());
            } else
                throw std::runtime_error("Regex grouping is not correct!");
        });
    if (ids.empty() && vars.empty()) return;

    const string column_name{sm_var[1]};
    const vector<IDMap> pair_item = [&]() {
        vector<IDMap> items;
        const auto id =
            std::find_if(args.begin(), args.end(), [&args](const IDMap& item) {
                return item.key == item.name;
            });
        if (id == args.end()) {
            items.emplace_back(it.name, it.node, it.name, it.degree,
                               ids);
        }
        items.emplace_back(it.name, it.node, column_name, it.degree, vars,
                           it.output);
        return items;
    }();
    it.key = "NULL";
    args.insert(args.begin(), pair_item.begin(), pair_item.end());
}

void validator(map<string, vector<IDMap>>& keys) {
    std::for_each(keys.begin(), keys.end(), [](auto& p) {
        vector<IDMap> items;
        std::for_each(p.second.begin(), p.second.end(),
                      [&p, &items](IDMap& key) {
                          if (key.type == "multi")
                              transform_keys(key, items);
                          else if (key.type == "group")
                              rotate_keys(key, items);
                      });

        p.second.erase(std::remove_if(p.second.begin(), p.second.end(),
                                      [&p, &items](const IDMap& key) {
                                          return key.key == "NULL";
                                      }),
                       p.second.end());
        p.second.insert(p.second.end(), items.begin(), items.end());
        items.clear();
    });
}

auto XMLParser::to_csv(string dir) -> bool {
    map<string, vector<IDMap>> keys;
    map<string, u_int32_t> key_sizes, level_sizes;
    transfrom_map(keys);
    validator(keys);
    size_t row = 0, col = 0;
    max_key_sizes(keys, key_sizes);

    std::stringstream ss;
    for (auto& key : keys) {
        level_sizes.insert(std::pair<std::string, u_int32_t>(
            key.first,
            std::max_element(key.second.begin(), key.second.end(),
                             [](const IDMap& a, const IDMap& b) -> bool {
                                 return a.values.size() < b.values.size();
                             })
                ->values.size()));
    }

    for (auto& p : keys) {
        std::ofstream ofs(dir + "/" + p.first + ".csv", std::ofstream::out);
        ss << write_header(p) << "\n";
        for (row = 0; row < level_sizes.at(p.first); row++) {
            for (col = 0; col < p.second.size() - 1; col++) {
                if (row < key_sizes.at(p.second.at(col).name +
                                       p.second.at(col).key)) {
                    ss << p.second.at(col).values.at(row) << DELIMETER;
                } else {
                    ss << DELIMETER;
                }
            }
            if (row <
                key_sizes.at(p.second.at(col).name + p.second.at(col).key)) {
                ss << p.second.at(col).values.at(row) << "\n";
            } else {
                ss << "\n";
            }
        }
        ofs << ss.str();
        ofs.close();
        ss.str(std::string());
    }
    return true;
}

auto inline XMLParser::write_header(
    const std::pair<const string, vector<IDMap>>& p) -> string {
    return std::string(std::accumulate(
        p.second.begin(), p.second.end(), std::string("INITIAL"),
        [](string entry, IDMap id) {
            string name;
            if (id.output.empty())
                name = id.key;
            else
                name = id.output;
            return entry != "INITIAL" ? entry + ";" + name : name;
        }));
}

void XMLParser::update(pugi::xml_node root, const IDMap& tag, svector& ids) {
    root = (!COUNTER) ? root.first_child() : root;
    COUNTER++;
    string token, id;
    for (pugi::xml_node panel = root.first_child(); panel;
         panel = panel.next_sibling()) {
        token = panel.name();
        if (token == tag.node &&
            string(panel.child_value()).find(tag.key) != string::npos) {
            CONTAINS = true;
            id = panel.child_value();
        }
        if (CONTAINS) {
            if (token.find(tag.degree) != string::npos) {
                ids.push_back(panel.child_value());
            }
        }
    }
    CONTAINS = false;
    root = root.next_sibling();
    if (root) update(root, tag, ids);
}

void XMLParser::operator()(string path, const string root_name) {
    for (const auto& file : listdir(path)) {
        pugi::xml_document doc;
        pugi::xml_node root{read(file, root_name, doc)};

        for (auto& tag : data) {
            if (tag.type == "single") {
                update(root, tag, tag.values);
            } else if (tag.type == "multi" || tag.type == "group") {
                const regex re{tag.key};

                update(root, tag, tag.map_values, re);

            } else {
                throw std::runtime_error(
                    "Wrong Type \n Defined Types: 'single', 'multi', 'group' ");
            }
            COUNTER = 0;
        }
    }
}

void XMLParser::update(pugi::xml_node root, const IDMap& tag,
                       map<string, vector<string>>& ids, const regex& re) {
    root = (!COUNTER) ? root.first_child() : root;
    COUNTER++;
    string token, id, child_value;
    smatch matches;
    for (pugi::xml_node panel = root.first_child(); panel;
         panel = panel.next_sibling()) {
        token = panel.name();
        child_value = static_cast<string>(panel.child_value());

        if (token == tag.node && regex_match(child_value, matches, re)) {
            CONTAINS = true;
            id = panel.child_value();
            if (ids.find(id) == ids.end())
                ids.insert(std::make_pair(id, vector<string>()));
        }
        if (CONTAINS) {
            if (token.find(tag.degree) != string::npos) {
                ids.find(id)->second.push_back(panel.child_value());
            }
        }
    }
    CONTAINS = false;
    root = root.next_sibling();
    if (root) update(root, tag, ids, re);
}