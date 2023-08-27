#pragma once
#ifndef NOSJ_H
#define NOSJ_H

#include <variant>
#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>
#include <charconv>
#include <regex>
#include <optional>

struct JSONObject;

using JSONDict = std::unordered_map<std::string, JSONObject>;
using JSONList = std::vector<JSONObject>;

struct JSONObject {
    std::variant<
        std::nullptr_t, // null
        bool,           // true/false
        int,            // 42
        double,         // 3.14
        std::string,    // "Diana"
        JSONList,    // [42, "Bella"]
        JSONDict // {"asoul": 42, "ee": 42}
    > inner;

    template <class T>
    bool is() const {
        return std::holds_alternative<T>(inner);
    }

    template <class T>
    T const & get() const {
        return std::get<T>(inner);
    }

    template <class T>
    T & get() {
        return std::get<T>(inner);
    }
};

struct JSONParser {
    std::pair<JSONObject, size_t> parse(std::string_view json) {
        if(json.empty()) {
            return {JSONObject{std::nullptr_t{}}, 0};
        } else if(size_t off = json.find_first_not_of(" \n\r\t\v\f\0"); off != 0 && off != json.npos) {
            auto [obj, eaten] = this->parse(json.substr(off));
            return {std::move(obj), eaten + off};
        } else if(('0' <= json[0] && json[0] <= '9') || json[0] == '+' || json[0] == '-') {
            std::regex num_regex{"[+-]?[0-9]+(\\.[0-9]*)?([eE][+-]?[0-9]+)?"};
            std::cmatch match;
            if(std::regex_search(json.data(), json.data()+json.size(), match, num_regex)) {
                std::string str = match.str();
                if(auto num = this->try_parse_num<int>(str)) {
                    return {JSONObject{*num}, str.size()};
                }
                if(auto num = this->try_parse_num<double>(str)) {
                    return {JSONObject{*num}, str.size()};
                }
            }
        } else if('"' == json[0]) { 
            std::string str;
            enum {Raw, Escaped} phase = Raw;
            size_t i;
            for(i = 1; i < json.size(); ++i) {
                char ch = json[i];
                if(phase == Raw) {
                    if(ch == '\\') {
                        phase = Escaped;
                    } else if(ch == '"') {
                        i += 1;
                        break;
                    } else {
                        str += ch;
                    }
                } else if(phase == Escaped) {
                    str += this->unescaped_char(ch);
                    phase = Raw;
                }
            }
            return {JSONObject{std::move(str)}, i};
        } else if(json[0] == '[') {
            JSONList res;
            size_t i;
            for(i = 1; i < json.size(); ) {
                if(auto emp = std::string(" \n\r\f\v\t\0"); emp.find(json[i]) != std::string::npos) {
                    i += 1;
                    continue;
                }
                if(json[i] == ']') {
                    i += 1;
                    break;
                }
                auto [obj, eaten] = this->parse(json.substr(i));
                if(eaten == 0) {
                    i = 0;
                    break;
                }
                res.push_back(std::move(obj));
                i += eaten;
                if(json[i] == ',') {
                    i += 1;
                }
            }
            return {JSONObject{std::move(res)}, i};
        } else if(json[0] == '{') {
            JSONDict res;
            size_t i;
            for(i = 1; i < json.size(); ) {
                if(auto emp = std::string(" \n\r\f\v\t\0"); emp.find(json[i]) != std::string::npos) {
                    i += 1;
                    continue;
                }
                if(json[i] == '}') {
                    i += 1;
                    break;
                }
                auto [keyobj, keyeaten] = this->parse(json.substr(i));
                if(keyeaten == 0) {
                    i = 0;
                    break;
                }
                i += keyeaten;
                if(!std::holds_alternative<std::string>(keyobj.inner)) {
                    i = 0;
                    break;
                }
                if(json[i] == ':') {
                    i += 1;
                }
                std::string key = std::move(std::get<std::string>(keyobj.inner));
                auto [valobj, valeaten] = this->parse(json.substr(i));
                if(valeaten == 0) {
                    i = 0;
                    break;
                }
                i += valeaten;
                res.try_emplace(std::move(key), std::move(valobj));
                if(json[i] == ',') {
                    i += 1;
                }
            } 
            return {JSONObject{std::move(res)}, i}; 
        } else if(json[0] == 'n' && json.size() >= 4) {
            if("null" == json.substr(0, 4)) {
                return {JSONObject{std::nullptr_t{}}, 4};
            }
        } else if(json[0] == 't' && json.size() >= 4) {
            if("true" == json.substr(0, 4)) {
                return {JSONObject{true}, 4};
            }
        } else if(json[0] == 'f' && json.size() >= 5) {
            if("false" == json.substr(0, 5)) {
                return {JSONObject{false}, 5};
            }
        }

        return {JSONObject{std::nullptr_t{}}, 0};
    }

    template<class T>
    std::optional<T> try_parse_num(std::string_view str) {
        T value;
        auto res = std::from_chars(str.data(), str.data() + str.size(), value);
        if(res.ec == std::errc() && res.ptr == str.data() + str.size()) {
            return value;
        }
        return std::nullopt;
    }

    char unescaped_char(char c) {
        switch(c) {
        case 'n': return '\n';
        case 'r': return '\r';
        case '0': return '\0';
        case 't': return '\t';
        case 'v': return '\v';
        case 'f': return '\f';
        case 'b': return '\b';
        case 'a': return '\a';
        default: return c;
        } 
    }

    std::string getJSONfromFile(std::string file_name) {
        std::string json;
        std::fstream file(file_name);
        if(file.is_open()) {
            std::string line;
            while(std::getline(file, line)) {
                json += line + "\n";
            }
            file.close();
        } else {
            json = "{}";
        }

        return json;
    }
};





#endif
