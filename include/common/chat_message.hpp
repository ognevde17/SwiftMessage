#pragma once
#include <string>

class ChatMessage {
public:
    ChatMessage() : chat_id(0), message_id(0) {}
    ChatMessage(int chat_id, int message_id) : chat_id(chat_id), message_id(message_id) {}
    
    int chat_id;
    int message_id;
}; 