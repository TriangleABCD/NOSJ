#include "nosj.h"

JSONObject JSONParser::parser(std::string_view json) {
    if(json.empty()) {
        return JSONObject{std::monostate{}};
    }
    if('0' <= json[0] && json[0] <= '9' 
        || json[0] == '+' || json[0] == '-) {
        std::regex num_regex{"[+-]?[0-9]+(\\.[0-9]*)?([eE][+-]?[0-9]+)?"};
        std::cmatch match;
        if(std::regex_search(json.data(), json.data()+json.size(), match, num_regex)) {
            std::string str = match.str();
            
        }
    }
    return JSONObject{std::nullptr_t{}};
    
}
