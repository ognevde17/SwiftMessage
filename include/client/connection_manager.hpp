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

  std::string receive(size_t length);

 private:
  boost::asio::io_context io_context;

  tcp::socket socket;
  std::string ip;
  std::string port;

  bool is_connected;
};