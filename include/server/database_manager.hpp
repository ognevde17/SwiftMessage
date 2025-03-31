#pragma once

#include <mutex>
#include <string>
#include <map>
#include "../../include/common/constants.hpp"

class DatabaseManager {
 public:
  static int GetClientIdByLogin(const std::string& login);
  
  static bool IsClientCorrectLoginAndPassword(const std::string& login, const std::string& password);

  static void AddClientIdByLogin(const std::string& login, int id);

 private:
  static std::map<int, std::string> client_id_to_login_;
  static std::map<std::string, int> client_login_to_id_;
};