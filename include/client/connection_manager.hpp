#include <boost/asio.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

using boost::asio::ip::tcp;

class ClientConnectionManager {
 public:
  ClientConnectionManager(std::string ip, const std::string& port);

  bool Connect();

  void Disconnect();

  bool send(const std::string& message);
  bool SendAuthRequest(const std::string& login, const std::string& password);
  const std::string& SendRegRequest(const std::string& login, const std::string& password);
  std::string receive(size_t length);
  bool is_connected;
 private:
  boost::asio::io_context io_context;
  tcp::socket socket;
  std::string ip;
  std::string port;
  
};