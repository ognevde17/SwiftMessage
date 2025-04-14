#pragma once
#include <string>

class Chat {
public:
    Chat() : id(0) {}
    
    int id;
    std::string name;
    std::string type;
}; 