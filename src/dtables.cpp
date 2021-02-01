#include "dtables.hpp"


using namespace dtable;



std::vector<string> Table::columns(){
        std::vector<string> vec;
        
        const YAML::Node& columns = file["COLUMNS"];
         
        for(YAML::const_iterator it = columns.begin(); it != columns.end(); it++ ){
            for(auto& kv:*it){
                vec.push_back(kv.first.as<string>());
            }
        }
    return vec;
}

dtable::Table::Table(string path): 
    path{path},file{open_yaml(path)}{
    validate();
    insert_column();
}


YAML::Node Table::open_yaml(string file_path){
    fs::path p(file_path);
    if(fs::exists(p)){
        return YAML::LoadFile(p);
    }
    throw std::runtime_error("File not exist at " +  file_path);
}



// template<typename T>
// bool Table::insert(string column, std::vector<T>){
//     for(auto column: data){             
//         column.
//     }



//     vector1.insert( vector1.end(), vector2.begin(), vector2.end() );    
// }

void Table::insert_column(){
   const YAML::Node& columns = file["COLUMNS"];
    string column;
    for(YAML::const_iterator it = columns.begin(); it != columns.end(); it++ ){
        for(auto& kv:*it){
            column = kv.second.as<string>();
            
            if(compare_column(column, "datetime")
               || compare_column(column, "float") 
               || compare_column(column, "int")){
               std::transform(column.begin()+1,
                                        column.end(), 
                                        column.begin()+1, ::tolower);
                auto row = std::make_shared<DataRow>();

                
                row->Dtype = column;                
                row->name = kv.first.as<string>(); // std::vector<string>
                data.push_back(std::move(row));
                }        
        }

    } 

}


void Table::info(){
    std::cout << "Columns\n";
    for(const auto row: data){
            std::cout<< "\tname: " << row->name<< "\n"
            << "\tdtype: " << row->Dtype << "\n"<< "\tsize: " << row->row.size() <<"\n";
        std::cout<< "\t" << string(20, '.') << "\n";
    }

    std::cout<< "Number of Columns: " << data.size() << "\n";
}



bool Table::compare_column(const string f, string s){
    string capitalized_s = s;
    string upper_s = s;    
    capitalized_s[0] = std::toupper(capitalized_s[0]);

    std::transform(capitalized_s.begin()+1,
                    capitalized_s.end(), 
                    capitalized_s.begin()+1, ::tolower);
    std::transform(upper_s.begin()+1,
                    upper_s.end(), 
                    upper_s.begin()+1, ::toupper);
    
    if(f == s 
        || f == upper_s 
        || f == capitalized_s){
            return true;
        }
        return false;
}

void Table::validate(){
    
    if(!file["COLUMNS"]){
        throw std::runtime_error("Configuration missing COLUMNS ");
    
}
}
