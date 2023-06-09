#ifndef NOSJ_H
#define NOSJ_H

#include <variant>
#include <vector>
#include <map>
#include <string>
#include <string_view>
#include <charconv>
#include <regex>

#include "print.h"

struct JSONObject {
    std::variant<
        std::monostate, // null
        bool,           // true
        int,            // 42
        double,         // 3.14
        std::string,    // "Diana"
        std::vector<JSONObject>,    // [42, "Bella"]
        std::map<std::string, JSONObject> // {"asoul": 42, "ee": 42}
    > inner;

    void do_print() const {
        print(inner);
    }
};

struct JSONParser {
    JSONObject parser(std::string_view json);

    template<class T>
    std::optional<T> try_parse_num(std::string_view str) {
        T value;
        auto res = std::from_chars(str.data(), str.data() + str.size(), value);
        if(res.ec == std::errc() && res.ptr == str.data() + str.size()) {
            return value;
        }
        return std::nullopt;
    }
};





#endif
