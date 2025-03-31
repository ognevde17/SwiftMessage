#include "../../include/server/database_manager.hpp"

std::mutex database_manager_mutex_;

std::map<int, std::string> DatabaseManager::client_id_to_login_;
std::map<std::string, int> DatabaseManager::client_login_to_id_;

int DatabaseManager::GetClientIdByLogin(const std::string& login) {
  std::lock_guard<std::mutex> lock(database_manager_mutex_);
  if (client_login_to_id_.find(login) == client_login_to_id_.end()) {
    throw std::runtime_error("Login not found: " + login);
  }
  return client_login_to_id_.at(login);
}

bool DatabaseManager::IsClientCorrectLoginAndPassword(
    const std::string& login, const std::string& password) {
  std::lock_guard<std::mutex> lock(database_manager_mutex_);
  return client_login_to_id_.find(login) != client_login_to_id_.end() &&
         client_id_to_login_[client_login_to_id_[login]] == password;
}

void DatabaseManager::AddClientIdByLogin(const std::string& login, int id) {
  std::lock_guard<std::mutex> lock(database_manager_mutex_);
  client_login_to_id_[login] = id;
}