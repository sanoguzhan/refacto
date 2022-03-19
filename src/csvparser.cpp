#include "refacto/csvparser.hpp"

#include <algorithm>
#include <boost/algorithm/string/join.hpp>
#include <numeric>
#include <regex>
#include <vector>


CSVParser::CSVParser(const string& path, unsigned int skip_rows_, const string& delim_)
  : file_path{ path }, f(validate_f(path)), skip_rows{ skip_rows_ }, parser{ f }, delim{ delim_ }, data{ read(skip_rows_) }
{
  file_name = get_substring(NAV, ".", path);
}

string CSVParser::validate_f(string path) const
{
  std::ifstream file(path);
  if (file.is_open()) { return path; }
  throw std::runtime_error("CSV not exist at " + file_path);
}

std::vector<std::vector<string>> CSVParser::read(int skip)
{
  std::vector<std::vector<string>> vec;
  parser.delimiter(delim[0]);
  int to_skip = 0;

  for (auto iter_row = parser.begin(); iter_row != parser.end(); iter_row++) {
    if (to_skip != skip) {
      to_skip++;
      continue;
    }
    vec.push_back(*iter_row);
  }
  return vec;
}

void CSVParser::erase_data(const string &orient, int32_t start, int32_t end)
{
  if (orient == "row") {
    data.erase(data.begin() + start, data.begin() + end);
  } else if (orient == "column") {
    for (auto &col : data) { col.erase(col.begin() + start, col.begin() + end); }
  }
}

void CSVParser::erase_diverge_row()
{
  size_t columns_number = data[0].size();
  data.erase(
    std::remove_if(
      data.begin(), data.end(), [&columns_number](auto columns) { return (columns.size() != columns_number); }),
    data.end());
}

void CSVParser::erase_pattern(const string& orient, string pattern)
{
  std::regex re(pattern);
  std::smatch matches;
  if (orient == "row") {
    data.erase(std::remove_if(data.begin(),
                 data.end(),
                 [&matches, &re](auto x) {
                   auto c = boost::algorithm::join(x, ";");
                   return std::regex_match(c, matches, re);
                 }),
      data.end());
  } else if (orient == "column") {
    std::vector<bool> colToRemove;
    for (auto header = data[0].begin(); header != data[0].end(); header++)
      colToRemove.push_back(std::regex_match(*header, matches, re));

    for (auto &row : data) {
      auto itrColToRm = colToRemove.begin();
      row.erase(std::remove_if(row.begin(),
                  row.end(),
                  [&itrColToRm](auto data) {
                    bool res = *itrColToRm;
                    itrColToRm++;
                    return res;
                  }),
        row.end());
    }
  }
}

Series CSVParser::operator()(const string& orient, u_int32_t idx, const Loc &target, const Loc &cond1, const Loc &cond2) const
{
  std::vector<string> rows;
  Series series{ .name = cond1.name };

  if (orient == "row") {
    std::vector<u_int32_t> data_idx{ row_search(data, cond1, cond2) };
    for (const u_int32_t col : data_idx) {
      for (size_t i = idx; i < data.size(); i++) { rows.push_back(data.at(i).at(col)); }
      series.values.insert({ data.at(target.row).at(col), rows });
      rows.resize(0);
    }
  }
  return series;
}

Series CSVParser::operator()(const string& orient, u_int32_t idx, const Loc &target, const Loc &cond1) const
{
  std::vector<string> rows;
  Series series;
  series.name = cond1.name;

  if (orient == "row") {
    std::vector<u_int32_t> data_idx{ row_search(data, cond1) };

    for (const u_int32_t col : data_idx) {
      for (size_t i = idx; i < data.size(); i++) { rows.push_back(data.at(i).at(col)); }
      series.values.insert({ data.at(target.row).at(col), rows });
      rows.resize(0);
    }
  }  return series;
}

Series CSVParser::operator()(const string& orient, const Loc &target, u_int32_t idx) const
{
  std::vector<string> rows;
  Series series;
  series.name = target.name;
  u_int32_t id_counter = 1;

  if (orient == "row") {
    std::vector<u_int32_t> data_idx{ row_search(data, target) };
    for (const u_int32_t col : data_idx) {
      for (size_t i = idx; i < data.size(); i++) { rows.push_back(data.at(i).at(col)); }
      series.values.insert({ std::to_string(id_counter), rows });
      rows.resize(0);
      id_counter++;
    }
  }
  return series;
}

std::vector<std::string> CSVParser::operator()(const std::string& orient, int32_t idx, int32_t from, int32_t to) const
{
  std::vector<string> row;
  int32_t counter = 0;
  if (orient == "row") {
    if (to == -1) to = data.at(idx).size();
    std::vector<string> cols(data.at(idx).begin() + from, data.at(idx).begin() + to);
    return cols;
  } else if (orient == "column") {
    if (to == -1) to = data.size();
    for (const auto &col : data) {
      if (counter == to) break;
      if (counter >= from) row.push_back(col.at(idx));
      counter++;
    }
  } else {
    throw std::runtime_error("Orient is missing!");
  }
  return row;
}

std::vector<u_int32_t> row_search(const std::vector<std::vector<string>> &data, const Loc &cond1, const Loc &cond2)
{
  string lookup1 = NULL;
  string lookup2 = NULL;
  std::vector<u_int32_t> indexes;

  for (size_t c = 0; c < data.at(0).size(); c++) {
    lookup1 = data.at(cond1.row).at(c);
    lookup2 = data.at(cond2.row).at(c);
    if (lookup1.find(cond1.name) != string::npos && lookup2.find(cond2.name) != string::npos) { indexes.push_back(c); }
  }
  return indexes;
}

 std::vector<u_int32_t> row_search(const std::vector<std::vector<string>> &data, const Loc &cond1)
{
  string lookup1 = NULL;
  const std::vector<u_int32_t> indexes = [&]() {
    std::vector<u_int32_t> index_values;
    for (size_t c = 0; c < data.at(0).size(); c++) {
      lookup1 = data.at(cond1.row).at(c);
      if (lookup1.find(cond1.name) != string::npos) { index_values.push_back(c); }
    }
    return index_values;
  }();
  return indexes;
}

std::string CSVParser::get_substring(std::string delimiter, std::string extention, std::string s) const
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

bool CSVParser::to_csv(string path) const
{
  std::ofstream ofs(path, std::ofstream::out);
  for (const auto &row_data : data) {
    ofs << std::accumulate(row_data.begin(), row_data.end(), std::string("START/FILE"), [](const auto &a, const auto &b) {
      return a != "START/FILE" ? a + ";" + b : b;
    }) << std::endl;
  }
  ofs.close();
  return true;
}