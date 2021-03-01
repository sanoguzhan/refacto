#include "csvparser.hpp"


using s_vector = const std::vector<std::vector<string>>;


CSVParser::CSVParser(string path, string delim, int skip_rows): 
    file_path{path}, f(validate_f(path)),
    parser{f}, delim{delim}, 
    skip_rows{skip_rows},data{read(skip_rows)}{
        file_name = get_substring(NAV, ".", path);
    }

CSVParser::CSVParser(string path, string delim): 
    file_path{path}, f(validate_f(path)),
    parser{f}, delim{delim},data{read(skip_rows)}{
         file_name = get_substring(NAV, ".", path);
    }

CSVParser::CSVParser(string path, int skip_rows): 
    file_path{path}, f(validate_f(path)), 
    skip_rows{skip_rows}, parser{f}, 
    data{read(skip_rows)}{
         file_name = get_substring(NAV, ".", path);
    }

CSVParser::CSVParser(string path): 
    file_path{path}, f(validate_f(path)), 
    parser{f}, data{read(skip_rows)}{
         file_name = get_substring(NAV, ".", path);
    }



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

Series CSVParser::values(string orient,
                        u_int32_t idx,
                        const Loc& target,
                        const Loc& cond1,
                        const Loc& cond2){

    std::vector<string> rows;
    Series series{
        .name = cond1.name
    }; 


    if(orient == "row"){
        std::vector<u_int32_t> data_idx{row_search(data, cond1, cond2)};
        for(const u_int32_t col: data_idx){
            for(size_t i=idx;i<data.size(); i++){                        
                rows.push_back(data.at(i).at(col));                        
            }
            series.values.insert({data.at(target.row).at(col), rows});
            rows.resize(0);
        }

    }else if(orient == "column"){
        //

    }
    return series;            
}

Series CSVParser::values(string orient,
                        u_int32_t idx,
                        const Loc& target,
                        const Loc& cond1){

    std::vector<string> rows;
    Series series{
        .name = cond1.name
    }; 


    if(orient == "row"){
        std::vector<u_int32_t> data_idx{row_search(data, cond1)};
        
        for(const u_int32_t col: data_idx){
            for(size_t i=idx;i<data.size(); i++){                        
                rows.push_back(data.at(i).at(col));                        
            }
            series.values.insert({data.at(target.row).at(col), rows});
            rows.resize(0);
        }

    }else if(orient == "column"){
        //

    }
    return series;            
}


Series CSVParser::values(string orient,
                        const Loc& target,
                        u_int32_t idx){
    std::vector<string> rows;
    Series series{
        .name = target.name
    }; 
    u_int32_t id_counter =1;

        if(orient == "row"){
            std::vector<u_int32_t> data_idx{row_search(data, target)};
            for(const u_int32_t col: data_idx){
                for(size_t i=idx;i<data.size(); i++){                        
                    rows.push_back(data.at(i).at(col));                        
                }
                series.values.insert({to_string(id_counter), rows});
                rows.resize(0);
                id_counter++;
            }

            }else if(orient == "column"){
                //
            }

    return series;            
}



std::vector<std::string> CSVParser::values(std::string orient,
                                            int32_t idx,
                                            int32_t from,
                                            int32_t to){
    std::vector<string> row;
    int32_t counter = 0;
    if(orient == "row"){
        if(to == -1 ) to = data.at(idx).size();
        std::vector<string>cols(data.at(idx).begin() +from, data.at(idx).begin() +to);
        return cols;

    }else if(orient == "column"){
        if(to == -1 ) to = data.size();
        for(const auto& col: data){
            if(counter == to) break;
            if(counter >= from) row.push_back(col.at(idx));
            counter++;
        }
    }else{
        throw std::runtime_error("Orient is missing!"); 
    }
    return row;
}





u_vector row_search(s_vector& data, 
                                    const Loc& cond1,
                                    const Loc& cond2){
            
    string lookup1, lookup2;
    std::vector<u_int32_t> indexes;
    
    for(size_t c=0; c < data.at(0).size(); c++){
        lookup1 = data.at(cond1.row).at(c);
        lookup2 = data.at(cond2.row).at(c);
        if(lookup1.find(cond1.name) != string::npos
            && lookup2.find(cond2.name) != string::npos){
            indexes.push_back(c);
            }
    }
    return indexes;
}

u_vector row_search(s_vector& data, 
                                   const Loc& cond1){
            
    string lookup1;
    std::vector<u_int32_t> indexes;
    for(size_t c=0; c < data.at(0).size(); c++){
        lookup1 = data.at(cond1.row).at(c);
        if(lookup1.find(cond1.name) != string::npos){
            indexes.push_back(c);
            }
    }
    return indexes;
}


std::string CSVParser::get_substring(std::string delimiter, std::string extention, std::string s){
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