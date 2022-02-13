#include "refacto/dtables.hpp"

using namespace dtable;

std::vector<string> Tables::columns() {
    std::vector<string> vec;

    const YAML::Node &columns = file["COLUMNS"];

    for (YAML::const_iterator it = columns.begin(); it != columns.end(); ++it) {
        for (auto &kv : *it) {
            vec.push_back(kv.first.as<string>());
        }
    }
    return vec;
}

dtable::Tables::Tables(string path) : path{path}, file{open_yaml(path)} {
    validate();
    insert_column();
}

YAML::Node Tables::open_yaml(string file_path) {
    fs::path p(file_path);
    if (fs::exists(p)) {
        return YAML::LoadFile(p);
    }
    throw std::runtime_error("File not exist at " + file_path);
}

void Tables::insert_column() {
    const YAML::Node &columns = file["COLUMNS"];
    string column;
    for (YAML::const_iterator it = columns.begin(); it != columns.end(); ++it) {
        for (auto &kv : *it) {
            column = kv.second.as<string>();

            if (compare_column(column, "datetime") ||
                compare_column(column, "float") ||
                compare_column(column, "int")) {
                std::transform(column.begin() + 1, column.end(),
                               column.begin() + 1, ::tolower);
                auto row = std::make_shared<DataRow>();

                row->Dtype = column;
                row->name = kv.first.as<string>();  // std::vector<string>
                data.push_back(std::move(row));
            }
        }
    }
}

void Tables::info() const {
    std::cout << "Columns\n";
    for (const auto &row : data) {
        std::cout << "\tname: " << row->name << "\n"
                  << "\tdtype: " << row->Dtype << "\n"
                  << "\tsize: " << row->row.size() << "\n";
        std::cout << "\t" << string(20, '.') << "\n";
    }

    std::cout << "Number of Columns: " << data.size() << "\n";
}

bool Tables::compare_column(const string f, string s) {
    string capitalized_s = s;
    string upper_s = s;
    capitalized_s[0] = std::toupper(capitalized_s[0]);

    std::transform(capitalized_s.begin() + 1, capitalized_s.end(),
                   capitalized_s.begin() + 1, ::tolower);
    std::transform(upper_s.begin() + 1, upper_s.end(), upper_s.begin() + 1,
                   ::toupper);

    if (f == s || f == upper_s || f == capitalized_s) {
        return true;
    }
    return false;
}

bool Tables::is_column(string token) {
    bool contains = false;
    for (const auto &col : data) {
        contains = compare_column(col->name, token);
    }
    return contains;
}

void Tables::validate() const {
    if (!file["COLUMNS"]) {
        throw std::runtime_error("Configuration missing COLUMNS ");
    }
}

std::vector<string> Tables::operator[](const string key) const {
    auto found{std::find_if(data.begin(), data.end(), [&key](const auto &col) {
        return col->name == key;
    })};
    if (found != std::end(data)) {
        return (*found)->row;
    }
    throw std::runtime_error("Column not exist");
}

std::shared_ptr<DataRow> Tables::get_column(const string &name) const {
    auto is_name = [name](const auto &row) { return row->name == name; };
    auto found = std::find_if(std::begin(data), std::end(data), is_name);
    if (*found != nullptr) {
        return *found;
    }

    throw std::runtime_error("Column not exist");
}

void Tables::save() const {
    std::ofstream csv_file;
    csv_file.open("test.csv");
    size_t i = 0;
    for (; i < data.size() - 1; i++) {
        csv_file << data.at(i)->name << ";";
    }
    csv_file << data.at(i)->name << std::endl;
    i = 0;
    for (; i < get_size(); i++) {
        size_t counter;
        counter = 0;
        for (const auto &item : data) {
            counter++;
            if (item->row.empty()) {
                csv_file << ";";
                if (counter == data.size())
                    csv_file << item->row.at(i) << std::endl;
                continue;
            }

            if (counter == data.size()) {
                csv_file << item->row.at(i) << std::endl;
                continue;
            }
            csv_file << item->row.at(i) << ";";
        }
    }
    csv_file.close();
}

bool Tables::insert(std::string target_column, std::string id_column,
                    Series series) {
    if (is_column(id_column) && is_column(series.name)) {
        return false;
    }
    auto id_col = get_column(id_column);
    auto val_col = get_column(target_column);
    std::vector<string>::iterator iter;
    for (const auto &p : series.values) {
        iter =
            std::find(std::begin(id_col->row), std::end(id_col->row), p.first);
        val_col->row.insert(std::end(val_col->row), std::begin(p.second),
                            std::end(p.second));
        if (iter == id_col->row.end())
            id_col->row.insert(std::end(id_col->row), p.second.size(), p.first);
    }
    return true;
}

bool Tables::insert(std::string key, std::vector<string> vec) {
    for (auto column : data) {
        if (column->name == key) {
            column->row.insert(std::end(column->row), std::begin(vec),
                               std::end(vec));
            return true;
        }
    }
    return false;
}