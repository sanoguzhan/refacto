#include "dtables.hpp"

using namespace table;

typedef std::map<std::string, std::vector<std::pair<std::string, std::vector<string>>>>::iterator map_iterator;
typedef std::vector<std::pair<string, std::vector<string>>> pair_vector;


bool Table::save(string path)
{
    
    std::ofstream csv_file;
    csv_file.open(path);
    size_t i = 0, loc = 0;
    size_t size = get_size();
    bool header = true;
    bool id_col = true;

    csv_file << "id;";
    for (const auto &c : data)
    {
        std::cout << c.first << std::endl;
        if (header)
        {
            for (; i < c.second.size() - 1; i++)
            {
                csv_file << c.second.at(i).first << ";";
            }
            csv_file << c.second.at(i).first << std::endl;
            header = false;
        }
        for (; loc < size; loc++, i = 0)
        {
                // std::cout << c.second.size() << std::endl;

            for (i = 0; i < c.second.size() - 1; i++)
            {
                if (id_col)
                    csv_file << c.first << ";" << c.second.at(i).second.at(loc) << ";";
                else
                    csv_file << c.second.at(i).second.at(loc) << ";";
                id_col = false;
            }
            id_col = true;
            csv_file << c.second.at(i).second.at(loc) << std::endl;
        }
        loc = 0;
    }
    csv_file.close();
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
