#include "table.hpp"

#include "dtables.hpp"
using namespace table;

typedef std::map<std::string,
                 std::vector<std::pair<std::string, std::vector<string>>>>::
    iterator map_iterator;
typedef std::vector<std::pair<string, std::vector<string>>> pair_vector;

static string DELIMETER = ";";

bool Table::save(string path) const {
    std::ofstream csv_file;
    csv_file.open(path);
    size_t i = 0, loc = 0;
    size_t size;
    bool id_col = true;

    std::stringstream ss;
    // write headers
    csv_file << "id;";
    for (; i < data.begin()->second.size() - 1; i++)
        ss << data.begin()->second.at(i).first << ";";
    ss << data.begin()->second.at(i).first << "\n";

    for (const auto &c : data) {
        size = data.at(c.first).front().second.size();
        if (c.second.size() <= 1) {
            for (; loc < size; loc++, i = 0) {
                try {
                    ss << c.first << DELIMETER << c.second.at(i).second.at(loc)
                       << "\n";
                } catch (const std::out_of_range) {
                    ss << "\n";
                }
            }
        } else {
            for (; loc < size; loc++, i = 0) {
                for (i = 0; i < c.second.size() - 1; i++) {
                    if (id_col) {
                        try {
                            ss << c.first << DELIMETER
                               << c.second.at(i).second.at(loc) << DELIMETER;
                        } catch (const std::out_of_range) {
                            ss << DELIMETER;
                        }
                    } else {
                        try {
                            ss << c.second.at(i).second.at(loc) << DELIMETER;
                        } catch (const std::out_of_range) {
                            ss << DELIMETER;
                        }
                    }
                    id_col = false;
                }
                id_col = true;
                try {
                    ss << c.second.at(i).second.at(loc) << "\n";
                } catch (const std::out_of_range) {
                    ss << "\n";
                }
            }
        }
        loc = 0;
    }
    csv_file << ss.str();
    csv_file.close();
    ss.str(std::string());
    return true;
}

bool Table::insert(Series series) {
    for (const auto &p : series.values) {
        if (data.find(p.first) == data.end()) {
            data.insert(
                {p.first, pair_vector{make_pair(series.name, p.second)}});
        } else if (!column_exist(p.first, series.name)) {
            data.at(p.first).push_back(make_pair(series.name, p.second));
        } else if (data.find(p.first) != data.end()) {
            auto found = std::find_if(
                data.find(p.first)->second.begin(),
                data.find(p.first)->second.end(),
                [&series](const auto &p) { return p.first == series.name; });
            found->second.insert(std::end(found->second), p.second.begin(),
                                 p.second.end());
        }
    }
    return true;
}

bool Table::insert(string name, std::vector<string> vec) {
    for (const auto &p : data) {
        if (!column_exist(p.first, name)) {
            data.at(p.first).push_back(make_pair(name, vec));
        } else {
            auto found = std::find_if(
                data.find(p.first)->second.begin(),
                data.find(p.first)->second.end(),
                [&name](const auto &p) { return p.first == name; });
            found->second.insert(std::end(found->second), vec.begin(),
                                 vec.end());
        }
    }
    return true;
}
bool Table::insert(string name, string id, std::vector<string> vec) {
    for (const auto &p : data) {
        if (!column_exist(p.first, name)) {
            data.at(p.first).push_back(make_pair(name, vec));
        } else {
            if (p.first == id) {
                auto found = std::find_if(
                    data.find(p.first)->second.begin(),
                    data.find(p.first)->second.end(),
                    [&name](const auto &p) { return p.first == name; });
                found->second.insert(std::end(found->second), vec.begin(),
                                     vec.end());
            }
        }
    }
    return true;
}

bool Table::insert(string name, string insert_name) {
    for (const auto &p : data) {
        if (!column_exist(p.first, name)) {
            data.at(p.first).push_back(make_pair(
                name,
                std::vector<string>(data.at(p.first).front().second.size(),
                                    insert_name)));
        } else {
            if (get_size(p.first, name) <= max_size()) {
                auto found = std::find_if(
                    data.find(p.first)->second.begin(),
                    data.find(p.first)->second.end(),
                    [&name](const auto &p) { return p.first == name; });
                found->second.insert(std::end(found->second),
                                     max_size() - get_size(p.first, name),
                                     insert_name);
            }
        }
    }
    return true;
}

size_t Table::max_size() const {
    size_t size = 0;
    for (const auto &c : data) {
        for (const auto &item : c.second) {
            if (size < item.second.size()) {
                size = item.second.size();
            }
        }
    }
    return size;
}

size_t Table::get_size(string id_name, string column_name) const {
    size_t size = 0;
    for (const auto &c : data) {
        for (const auto &item : c.second) {
            if (id_name == c.first && item.first == column_name) {
                size = item.second.size();
            }
        }
    }
    return size;
}

bool Table::column_exist(const string &id, const string &col_name) const {
    return std::any_of(data.at(id).begin(), data.at(id).end(),
                       [&](const auto &p) { return col_name == p.first; });
}
