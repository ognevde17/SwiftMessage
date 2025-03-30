#include "../../include/client/connection_manager.hpp"

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
    boost::asio::connect(socket, resolver.resolve(ip, port));
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