#pragma once
#include <string>

class Chat {
public:
    int id;
    std::string name;
    std::string type;  // "private" или "group"
}; 