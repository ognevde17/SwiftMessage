#pragma once

#include <atomic>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <thread>
#include <utility>

#include "../common/messages.hpp"
#include "connection_manager.hpp"
#include "database_manager.hpp"
#include "request_manager.hpp"

class Server {
 public:
  static Server& GetInstance();

  void Run();


  Server(const Server&) = delete;
  Server& operator=(const Server&) = delete;

 private:
  Server();
  ~Server() = default;

  void Session(int user_id);

  boost::asio::io_context io_context_;
  ConnectionManager connection_manager_;
  DatabaseManager database_manager_;
  RequestManager request_manager_;
};
