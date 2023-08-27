#include <iostream>
#include <fstream>
#include <assert.h>

#include "nosj.h"

int main(int argc, char** argv) {
    assert(argc >= 2);
    JSONParser jsonParser;
    std::string json = jsonParser.getJSONfromFile(std::string(argv[1]));
    auto [obj, eaten] = jsonParser.parse(json);
    std::cout << std::boolalpha << obj.get<JSONDict>().at("isTrue").get<bool>() << std::endl;
    std::cout << std::boolalpha << obj.get<JSONDict>().at("done").get<bool>() << std::endl;
    std::cout << obj.get<JSONDict>().at("age").get<std::nullptr_t>() << std::endl;
    return 0;
}
