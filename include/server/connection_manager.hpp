#pragma once

#include <atomic>
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <thread>
#include <utility>

#include "../../include/common/constants.hpp"

using boost::asio::ip::tcp;

class ConnectionManager {
 public:
  ConnectionManager();

  int AcceptNewClient();

  std::string ReceiveData(int client_id);

  static bool SendData(int client_id, const std::string& data);

 private:
  int GenerateClientId();

  void CloseConnection(tcp::socket& socket);

  void AddClientSocket(int client_id, tcp::socket socket);

  static tcp::socket& GetClientSocket(int client_id);

  void RemoveClientSocket(int client_id);

  std::string ReceiveData(tcp::socket& socket);

  static bool SendData(tcp::socket& socket, const std::string& data);

  boost::asio::io_context io;

  tcp::acceptor acceptor_;

  std::atomic<int> client_id_counter_{1};

  static std::map<int, tcp::socket> client_id_to_socket_;
};
