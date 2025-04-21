#include "../../include/client/connection_manager.hpp"
#include "../../include/common/messages.hpp"

ClientConnectionManager::ClientConnectionManager(std::string ip,
                                                 const std::string& port)
    : io_context(),
      socket(io_context),
      ip(ip),
      port(port),
      is_connected(false) {}

bool ClientConnectionManager::Connect() {
  try {
    tcp::resolver resolver(io_context);
    std::cout << "OK1" << std::endl;
    boost::asio::connect(socket, resolver.resolve(ip, port));
    std::cout << "OK2" << std::endl;
    is_connected = true;
    return true;
  } catch (...) {
    is_connected = false;
    return false;
  }
}

void ClientConnectionManager::Disconnect() {
  socket.close();
  is_connected = false;
}

bool ClientConnectionManager::send(const std::string& message) {
  try {
    boost::asio::write(socket, boost::asio::buffer(message));
    return true;
  } catch (...) {
    std::cerr << "send fail\n";
    return false;
  }
}

std::string ClientConnectionManager::receive(size_t length) {
  try {
    std::vector<char> reply(length);
    size_t reply_length = socket.read_some(boost::asio::buffer(reply));
    return std::string(reply.data(), reply_length);
  } catch (std::exception& e) {
    std::cerr << "receive fail\n";
    throw;
  }
}
bool ClientConnectionManager::SendAuthRequest(const std::string& login, const std::string& password) {
  try {
    AuthRequest request;
    request.login = login;
    request.password = password;
    boost::asio::write(socket, boost::asio::buffer(request.to_string()));
    std::vector<char> reply(1024);
    size_t reply_length = socket.read_some(boost::asio::buffer(reply));
    std::string server_reply = std::string(reply.data(), reply_length);
    ServerResponse response = ServerResponse::from_string(server_reply);
    if (response.response_text == "AUTH_SUCCESS") {
      return true;
    } else return false;
  } catch (...) {
    return false;
  }
}
std::string ClientConnectionManager::SendRegRequest(const std::string& login, const std::string& password) {
  try {
    RegisterRequest request;
    request.login = login;
    request.password = password;
    boost::asio::write(socket, boost::asio::buffer(request.to_string()));
    std::vector<char> reply(1024);
    size_t reply_length = socket.read_some(boost::asio::buffer(reply));
    std::string server_reply = std::string(reply.data(), reply_length);
    ServerResponse response = ServerResponse::from_string(server_reply);
    if (response.response_text == "You have successfully registered") {
      return "You have successfully registered";
    } else if (response.response_text == "This login already exists"){
      return "ERROR1";
    } else {
      return "ERROR2";
    }
  } catch (...) {
    return "ERROR2";
  }
}