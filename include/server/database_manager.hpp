#pragma once

#include <memory>
#include <pqxx/pqxx>
#include <string>
#include <vector>

#include "../common/chat.hpp"
#include "../common/message.hpp"
#include "../common/user.hpp"
#include "../common/chat_participant.hpp"
#include "../common/chat_message.hpp"

class DatabaseManager {
 public:
  DatabaseManager(const std::string& connection_string);
  ~DatabaseManager();

  // Управление пользователями
  bool IsClientCorrectLoginAndPassword(const std::string& login,
                                       const std::string& password);
  bool CreateUser(const std::string& login, const std::string& password);
  bool AuthenticateUser(const std::string& login, const std::string& password);
  bool IsClientLoginExists(const std::string& login);
  std::vector<User> GetUsers();
  User GetUserById(int user_id);
  int GetClientIdByLogin(const std::string& login);

  // Управление чатами
  bool CreateChat(int user_id1, int user_id2);
  std::vector<Chat> GetUserChats(int user_id);
  // Новые методы для работы с ChatParticipant
  bool AddUserToChat(int user_id, int chat_id);
  std::vector<Chat> GetChatsByUserLogin(const std::string& login);

  // Управление сообщениями
  bool SaveMessage(const Message& message);
  std::vector<Message> GetChatMessages(int chat_id);
  // Новые методы для работы с ChatMessage
  bool LinkMessageToChat(int chat_id, int message_id);
  std::vector<Message> GetMessagesByChatId(int chat_id);

 private:
  std::unique_ptr<pqxx::connection> db_connection;
};
