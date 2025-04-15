#pragma once
#include <string>

class Message {
public:
    Message() : id(0), sender_id(0), receiver_id(0), chat_id(0), 
               is_read(false), is_edited(false) {}
    
    int id;
    int sender_id;
    int receiver_id;
    int chat_id;
    std::string content;
    bool is_read;
    bool is_edited;
    std::string sent_at;
}; 