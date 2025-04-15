#pragma once

#include <atomic>
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <unordered_map>
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

  std::string ReceiveData(int connection_id);

  static bool SendData(int connection_id, const std::string& data);

 private:
  // Методы:

  int GenerateConnectionId();

  void CloseConnection(tcp::socket& socket);

  void AssociateConnectionIdWithSocket(int connection_id, tcp::socket socket);
  void AssociateLoginWithConnectionId(const std::string& login, int connection_id);

  void RemoveAssociationBetweenConnectionIdAndSocket(int connection_id);
  void RemoveAssociationBetweenLoginAndConnectionId(const std::string& login);

  std::string ReceiveData(tcp::socket& socket);

  static bool SendData(tcp::socket& socket, const std::string& data);

  // Поля:

  boost::asio::io_context io;

  tcp::acceptor acceptor_;

  std::atomic<int> connection_id_counter_{1};

  static std::unordered_map<int, tcp::socket> connection_id_to_socket_;
};
