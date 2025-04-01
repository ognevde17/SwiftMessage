#pragma once
#include <string>

class Message {
public:
    int id;
    int sender_id;
    int receiver_id;
    int chat_id;
    std::string content;
    bool is_read;
    bool is_edited;
    std::string sent_at;
}; 