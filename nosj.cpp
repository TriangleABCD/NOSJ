#include "nosj.h"

JSONObject JSONParser::parser(std::string_view json) {
    if(json.empty()) {
        return JSONObject{std::monostate{}};
    }
    if('0' <= json[0] && json[0] <= '9') {
        
    }
}
