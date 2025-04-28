#pragma once
#include <string>

class ChatParticipant {
public:
    ChatParticipant() : user_id(0), chat_id(0) {}
    ChatParticipant(int user_id, int chat_id) : user_id(user_id), chat_id(chat_id) {}
    
    int user_id;
    int chat_id;
}; 