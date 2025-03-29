#pragma once

#include "../common/messages.hpp"
#include "database_manager.hpp"
#include "connection_manager.hpp"

class RequestManager {
 public:
  static void HandleRequest(const std::string& request);
};