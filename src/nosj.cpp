#include "nosj.h"

JSONObject JSONParser::parser(std::string_view json) {
    if(json.empty()) {
        return JSONObject{std::monostate{}};
    }
    if('0' <= json[0] && json[0] <= '9') {
        return JSONObject{int{json[0] - '0'}};        
    }
    return JSONObject{std::nullptr_t{}};
    
}
