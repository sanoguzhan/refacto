#include "csvparser.hpp"




string CSVParser::validate_f(string path){
        fs::path p(path);
        if(fs::exists(p)){
            return path;
    }
    throw std::runtime_error("CSV not exist at " +  file_path);
}

std::vector<std::vector<string>> CSVParser::read(int skip){
    std::vector<std::vector<string>> vec;
    auto field = parser.next_field();
    
    move_iter(field, skip);
    
    while(field.type != csv::FieldType::CSV_END){
        if(field.type == csv::FieldType::ROW_END){
            field = parser.next_field();
            continue;
        }
        vec.push_back(row_vector(static_cast<string>(*field.data)));
        field = parser.next_field();
    }
    return vec;
}    


void CSVParser::move_iter(csv::Field &field, int skip){
        int counter = 0;
            while(counter < skip-1){
                if(field.type == csv::FieldType::ROW_END){
                    counter++;
                    field = parser.next_field();
                    continue;
                } 
                field = parser.next_field();
                continue;
            }
}

std::vector<string> CSVParser::read_line(){
    long unsigned end, start = 0U;
    std::vector<string> vec;
    for(;;){
        auto field = parser.next_field();
        string s;
        if(field.type == csv::FieldType::ROW_END
            || field.type == csv::FieldType::CSV_END) continue;
        
        s = *field.data;                
        end = s.find(delim);

        while (end != std::string::npos){
            vec.push_back(static_cast<string>(s.substr(start, end - start)));
            start = end + delim.length();
            end = s.find(delim, start);
        }
        vec.push_back(static_cast<string>(s.substr(start, end)));
        break;
    }
    return vec;
}    

std::vector<u_int32_t> row_search(const std::vector<std::vector<string>>& data, 
                                    const Loc& token){
            string lookup;
            std::vector<u_int32_t> indexes;
            for(size_t r=0; r < data.size(); r++){
                lookup = data.at(token.row).at(r);
                if(lookup.find(token.name) != string::npos){
                    indexes.push_back(r);
                    }
            }
            return indexes;
        }