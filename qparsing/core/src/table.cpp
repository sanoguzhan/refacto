#include "dtables.hpp"
#include"table.hpp"
using namespace table;

typedef std::map<std::string, std::vector<std::pair<std::string, std::vector<string>>>>::iterator map_iterator;
typedef std::vector<std::pair<string, std::vector<string>>> pair_vector;


static string DELIMETER = ";";


bool Table::save(string path){
    
    std::ofstream csv_file;
    csv_file.open(path);
    size_t i = 0, loc = 0;
    size_t size{get_size()};
    bool id_col = true;

    std::stringstream ss;
    //write headers
    csv_file << "id;";
    for (; i < data.begin()->second.size() - 1; i++)
                ss << data.begin()->second.at(i).first << ";";
    ss << data.begin()->second.at(i).first << "\n";

    for (const auto &c : data){
        if(c.second.size() <= 1){
            for (; loc < size; loc++, i = 0){
                try{ss << c.first << DELIMETER << c.second.at(i).second.at(loc) << "\n";}
                catch(const std::out_of_range){ss << "\n";}
            }
        }else{
            for (; loc < size; loc++, i = 0){
                for (i = 0; i < c.second.size() - 1; i++){
                    if (id_col){
                        try{ss << c.first << DELIMETER << c.second.at(i).second.at(loc) << DELIMETER;}
                        catch(const std::out_of_range){ss << DELIMETER;}
                    }
                    else{
                        try{ss << c.second.at(i).second.at(loc) << DELIMETER;}
                        catch(const std::out_of_range){ss << DELIMETER;}
                    }
                id_col = false;
                }
            id_col = true;
            try{ss << c.second.at(i).second.at(loc) << "\n";}
            catch(const std::out_of_range){ss <<"\n";} 
            }
        }
        loc = 0;
    }
    csv_file << ss.str();
    csv_file.close();
    ss.str(std::string());
    return true;
}

bool Table::insert(Series series)
{
    for (const auto &p : series.values)
    {
        if (data.find(p.first) == data.end())
        {
            data.insert({p.first, pair_vector{make_pair(series.name, p.second)}});
        }
        else if (!column_exist(p.first, series.name))
        {
            data.at(p.first).push_back(make_pair(series.name, p.second));
        }
    }
    return true;
}

bool Table::insert(string name, std::vector<string> vec)
{
    for (const auto &p : data)
    {
        if (!column_exist(p.first, name))
        {
            data.at(p.first).push_back(make_pair(name, vec));
        }
    }
    return true;
}

bool Table::insert(string name, string insert_name)
{
    for (const auto &p : data)
    {
        if (!column_exist(p.first, name))
        {
            data.at(p.first).push_back(make_pair(name, std::vector<string>(get_size(), insert_name)));
        }
    }
    return true;
}

size_t Table::get_size() const
{
    size_t size = 0;
    for (const auto &c : data)
    {
        for (const auto &item : c.second)
        {
            if (size < item.second.size())
            {
                size = item.second.size();
            }
        }
    }
    return size;
}

bool Table::column_exist(const string &id, const string &col_name) const
{
    for (const auto &p : data.at(id))
    {
        if (col_name == p.first)
        {
            return true;
        }
    }
    return false;
}