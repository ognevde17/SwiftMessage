#pragma once

#include "../common/messages.hpp"
#include "database_manager.hpp"
#include "connection_manager.hpp"

class RequestManager {
 public:
  RequestManager();
  ~RequestManager() = default;

  void HandleRequest(DatabaseManager& database_manager, const std::string& request, const int connection_id);
 private:
  // Методы:
  void AssociateUserIdWithConnectionId(const int user_id, const int connection_id);
  int GetConnectionIdByUserId(const int user_id);

  void HandleRegisterRequest(DatabaseManager& database_manager, const std::string& request, const int connection_id);
  void HandleAuthRequest(DatabaseManager& database_manager, const std::string& request, const int connection_id);
  void HandleSendMessageRequest(DatabaseManager& database_manager, const std::string& request, const int connection_id);

  // Поля:
  // std::mutex user_id_to_connection_id_mutex_; // Commented out
  std::unordered_map<int, int> user_id_to_connection_id_;
};