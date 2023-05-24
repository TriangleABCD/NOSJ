#include <iostream>

#include "nosj.h"

int main(int argc, char** argv) {
    std::string json = "3";
    JSONParser jsonParser;
    print(jsonParser.parser(json));
    return 0;
}
