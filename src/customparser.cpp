#include "refacto/customparser.hpp"

deque<string> get_flist(string pattern)
{
  if (!is_path_exist(pattern)) { throw std::runtime_error("Folder not exist!"); }
  pattern += "*.csv";
  glob_t glob_result;
  glob(pattern.c_str(), GLOB_TILDE, NULL, &glob_result);
  deque<string> files;
  for (unsigned int i = 0; i < glob_result.gl_pathc; ++i) { files.push_back(string(glob_result.gl_pathv[i])); }
  globfree(&glob_result);
  return files;
}

void CustomParser::operator()(string dir_path, string delimeter, int skip)
{
  deque<string> files{ get_flist(dir_path) };
  map<string, vector<Entity>> keys;
  transform_keys(keys);
  Series series;
  vector<string> vec_values;

  for (auto file : files) {
    in_read(file, delimeter, skip);
    file_name = get_substring("/", ".", file);
    for (auto p : tables) {
      in_insert(p.second, keys.at(p.first), series, vec_values);
      series.values.clear();// clean the parsed values
      id_iter_vector.clear();
    }
    data.clear();
  }
}

string CustomParser::get_variable_name(const Entity &item) const
{
  std::regex e_name(item.conditions.at(0).at("name"));
  std::cmatch cm_name;
  for (size_t c = 0; c < data.at(item.row).size(); c++) {
    std::regex_match(data.at(item.row).at(c).c_str(), cm_name, e_name, std::regex_constants::match_default);
    if (cm_name.size() > 0) { return cm_name[1]; }
  }
  throw std::runtime_error("Regex is greedy, Column Not Found");
}

void CustomParser::in_insert(shared_ptr<Table> tb,
  const vector<Entity> &entity_lst,
  Series &series,
  vector<string> &vec_values)
{
  for (auto item : entity_lst) {
    if (item.eType == "ids") {
      create_ids(series, item);
    } else if (item.eType == "group") {
      from_group_series(series, item, tb);
    } else if (item.eType == "series") {
      from_series(series, item, tb);
    } else if (item.eType == "vector") {
      from_vector(vec_values, item);
      if (id_type == "single") {
        tb->insert(item.name, id_iter, vec_values);
      } else {
        tb->insert(item.name, vec_values);
      }
      vec_values.clear();
    } else if (item.eType == "entity") {
      if (item.keyword == "file_name")
        tb->insert(item.name, file_name);
      else
        tb->insert(item.name, item.keyword);
    } else {
      throw std::runtime_error("Wrong Configuration identifier!");
    }
  }
}

void CustomParser::from_group_series(Series &series, const Entity &item, shared_ptr<Table> tb)
{
  const auto conditions = item.conditions.at(0);
  const std::regex e_id(conditions.at("id"));

  std::smatch cm_id;
  series.name = get_variable_name(item);
  string _id;

  vector<string> rows;
  for (size_t c = 0; c < data.at(0).size(); c++) {

    std::regex_match(data.at(item.row).at(c), cm_id, e_id, std::regex_constants::match_default);
    if (cm_id.size() > 0) {
      _id = cm_id[1];
      if (conditions.find("parent_id_row") != conditions.end())
        _id = data.at(stoi(conditions.at("parent_id_row"))).at(c) + "-" + _id;

      for (size_t i = item.value_begin; i < data.size(); i++) { rows.push_back(data.at(i).at(c)); }
      if (!id_exist(series, _id)) {
        series.values.insert(std::make_pair(_id, rows));

      } else {
        series.values.find(_id)->second.insert(
          std::end(series.values.find(_id)->second), std::begin(rows), std::end(rows));
      }
      rows.clear();
    }
    tb->insert(series);
    clean_series(series);
  }
}

void CustomParser::from_vector(vector<string> &vec_values, Entity &item) const
{
  int32_t counter = 0;
  if (item.column > data.size()) throw std::runtime_error("Column index is bigger than data index");
  if (item.orient == "row") {
    if (item.vTo == -1) item.vTo = data.at(item.column).size();
    vec_values.insert(
      vec_values.end(), data.at(item.column).begin() + item.vFrom, data.at(item.column).begin() + item.vTo);
  } else if (item.orient == "column") {
    if (item.vTo == -1) item.vTo = data.size();
    for (const auto &col : data) {
      if (counter == item.vTo) break;
      if (counter >= item.vFrom) vec_values.push_back(col.at(item.column));
      counter++;
    }
  } else {
    throw std::runtime_error("Orient is missing!");
  }
}

void CustomParser::from_series(Series &series, const Entity &item, shared_ptr<Table> tb)
{
  if (item.conditions.empty()) {
    construct_series(series, item, tb);
  } else if (item.conditions.size() == 1) {
    construct_series(series, item, item.conditions.at(0), tb);
  } else if (item.conditions.size() == 2) {
    throw std::runtime_error("Multi conditions are not supported");
  } else {
    throw std::runtime_error("Wrong configurations");
  }
}

void CustomParser::create_ids(Series &series, const Entity &item)
{
  if (item.keyword == "file_name") {
    if (!id_exist(series, file_name)) {
      series.values.insert(std::make_pair(file_name, vector<string>()));
      id_type = "single";
      id_iter = file_name;
    }
  } else if (item.keyword == "single") {
    if (!id_exist(series, data.at(item.row).at(item.column))) {
      series.values.insert(std::make_pair(data.at(item.row).at(item.column), vector<string>()));
      id_type = "single";
      id_iter = data.at(item.row).at(item.column);
    }
  } else if (item.keyword == "multi") {
    vector<u_int32_t> idx_vector;
    find_ids(data, idx_vector, item);
    for (auto i : idx_vector) {
      if (!id_exist(series, data.at(item.value_begin).at(i))) {
        id_iter_vector.push_back(data.at(item.value_begin).at(i));
        series.values.insert(std::make_pair(data.at(item.value_begin).at(i), vector<string>()));
      }
    }
    id_type = "multi";
    idx_vector.clear();
  } else {
    throw std::runtime_error("Wrong id config");
  }
}

void CustomParser::construct_series(Series &series, const Entity &item, shared_ptr<Table> tb)
{
  vector<string> rows;

  // First Condition as name
  series.name = item.name;
  std::vector<u_int32_t> data_idx;
  find_ids(data, data_idx, item);
  if (series.values.empty()) throw std::runtime_error("ID not initilized correctly!");
  if (id_type == "single") {
    for (const u_int32_t col : data_idx) {
      for (size_t i = item.value_begin; i < data.size(); i++) { rows.push_back(data.at(i).at(col)); }
      series.values.find(id_iter)->second.insert(
        std::end(series.values.find(id_iter)->second), std::begin(rows), std::end(rows));
      rows.resize(0);
    }
    tb->insert(series);
  } else if (id_type == "multi") {
    if (id_iter_vector.size() != data_idx.size()) throw std::runtime_error("ID size not matched with variables");
    std::vector<string>::iterator id_iterator{ id_iter_vector.begin() };
    for (const u_int32_t col : data_idx) {
      for (size_t i = item.value_begin; i < data.size(); i++) { rows.push_back(data.at(i).at(col)); }
      series.values.find(*id_iterator)
        ->second.insert(std::end(series.values.find(*id_iterator)->second), std::begin(rows), std::end(rows));
      rows.resize(0);
      ++id_iterator;
    }
    tb->insert(series);
  }
  clean_series(series);
}

void CustomParser::construct_series(Series &series,
  const Entity &item,
  map<string, string> target,
  shared_ptr<Table> tb) const
{
  std::vector<string> rows;

  // First Condition as name
  series.name = item.name;

  std::vector<u_int32_t> data_idx;
  string current_id;
  find_ids(data, data_idx, target, item);
  for (const u_int32_t col : data_idx) {
    for (size_t i = item.value_begin; i < data.size(); i++) { rows.push_back(data.at(i).at(col)); }
    current_id = data.at(std::stoi(target.find("row")->second)).at(col);
    // Key not Exist so Initilize
    if (series.values.find(current_id) == series.values.end()) {
      series.values.insert({ current_id, rows });
    } else {
      series.values.find(current_id)
        ->second.insert(std::end(series.values.find(current_id)->second), std::begin(rows), std::end(rows));
    }
    rows.resize(0);
    tb->insert(series);
    clean_series(series);
  }
}

void CustomParser::transform_keys(map<string, vector<Entity>> &keys)
{
  vector<string> meta_names;
  std::sort(meta.begin(), meta.end());
  std::transform(meta.begin(), meta.end(), back_inserter(meta_names), [](Entity &id) { return id.key; });
  meta_names.erase(unique(meta_names.begin(), meta_names.end()), meta_names.end());

  // Groups all entities by key name
  transform(meta_names.begin(), meta_names.end(), std::inserter(keys, keys.end()), [](string &key) {
    return std::make_pair(key, vector<Entity>());
  });

  // Creates and group tables by key name
  transform(meta_names.begin(), meta_names.end(), std::inserter(tables, tables.end()), [](string &key) {
    return std::make_pair(key, make_shared<Table>());
  });

  for_each(meta.begin(), meta.end(), [&](Entity &id) { keys.at(id.key).emplace_back(id); });
}

string CustomParser::get_substring(std::string delimiter, std::string extention, std::string s) const
{
  std::string token;
  size_t pos = 0;
  while ((pos = s.find(delimiter)) != std::string::npos) {
    token = s.substr(0, pos);
    s.erase(0, pos + delimiter.length());
  }
  pos = 0;
  while ((pos = s.find(extention)) != std::string::npos) {
    token = s.substr(0, pos);
    s.erase(pos, token.length() + delimiter.length());
  }
  return s;
}

void find_ids(s_matrix &data, vector<u_int32_t> &indexes, const Entity &item)
{
  string lookup_id;
  std::regex reg_id{ item.name };
  for (size_t c = 0; c < data.at(0).size(); c++) {
    lookup_id = data.at(item.row).at(c);
    if (std::regex_match(lookup_id, reg_id)) indexes.push_back(c);
  }
}

void find_ids(s_matrix &data, vector<u_int32_t> &indexes, map<string, string> cond, const Entity &item)
{
  string lookup_var, lookup_id;
  std::regex reg_id{ cond.find("name")->second };
  std::regex reg_var{ item.name };
  for (size_t c = 0; c < data.at(0).size(); c++) {
    lookup_id = data.at(std::stoi(cond.find("row")->second)).at(c);
    lookup_var = data.at(item.row).at(c);
    if (std::regex_match(lookup_id, reg_id) && std::regex_match(lookup_var, reg_var)) indexes.push_back(c);
  }
}
