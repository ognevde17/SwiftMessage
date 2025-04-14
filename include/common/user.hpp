#pragma once
#include <string>

class User {
public:
    User() : id(0) {}
    
    int id;
    std::string username;
    std::string email;
    std::string last_seen_at;
}; 