#pragma once

#include <string>
#include <vector>
#include <memory>
#include <pqxx/pqxx>

// Предварительные объявления классов
class User;
class Chat;
class Message;

class DatabaseManager {
public:
    DatabaseManager(const std::string& connection_string);
    ~DatabaseManager();

    // Управление пользователями
    bool CreateUser(const std::string& login, const std::string& password);
    bool AuthenticateUser(const std::string& login, const std::string& password);
    std::vector<User> GetUsers();
    User GetUserById(int user_id);

    // Управление чатами
    bool CreateChat(int user_id1, int user_id2);
    std::vector<Chat> GetUserChats(int user_id);

    // Управление сообщениями
    bool SaveMessage(const Message& message);
    std::vector<Message> GetChatMessages(int chat_id);

private:
    std::unique_ptr<pqxx::connection> db_connection;
}; 