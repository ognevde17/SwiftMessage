#include "../../include/server/connection_manager.hpp"

std::mutex client_id_to_socket_mutex_;

std::map<int, tcp::socket> ConnectionManager::client_id_to_socket_;

ConnectionManager::ConnectionManager()
    : acceptor_(
          io, tcp::endpoint(tcp::v4(), Constants::SERVER_PORT_UNSIGNED_SHORT)) {
}

int ConnectionManager::AcceptNewClient() {
  tcp::socket socket = acceptor_.accept();

  int client_id = GenerateClientId();

  AddClientSocket(client_id, std::move(socket));

  return client_id;
}

std::string ConnectionManager::ReceiveData(int client_id) {
  return ReceiveData(GetClientSocket(client_id));
}

bool ConnectionManager::SendData(int client_id, const std::string& data) {
  return SendData(GetClientSocket(client_id), data);
}

int ConnectionManager::GenerateClientId() {
  return client_id_counter_.fetch_add(1);
}

void ConnectionManager::CloseConnection(tcp::socket& socket) { socket.close(); }

void ConnectionManager::AddClientSocket(int client_id, tcp::socket socket) {
  std::lock_guard<std::mutex> lock(client_id_to_socket_mutex_);
  ConnectionManager::client_id_to_socket_.emplace(client_id, std::move(socket));
}

tcp::socket& ConnectionManager::GetClientSocket(int client_id) {
  std::lock_guard<std::mutex> lock(client_id_to_socket_mutex_);
  return client_id_to_socket_.at(client_id);
}

void ConnectionManager::RemoveClientSocket(int client_id) {
  std::lock_guard<std::mutex> lock(client_id_to_socket_mutex_);
  client_id_to_socket_.erase(client_id);
}

std::string ConnectionManager::ReceiveData(tcp::socket& socket) {
  char data[Constants::MAX_LENGTH];

  boost::system::error_code error;

  size_t length = socket.read_some(boost::asio::buffer(data), error);

  if (error) {
    throw boost::system::system_error(error);
  }

  return std::string(data, length);
}

bool ConnectionManager::SendData(tcp::socket& socket, const std::string& data) {
  boost::system::error_code error;

  boost::asio::write(socket, boost::asio::buffer(data), error);

  if (error) {
    return false;
  }

  return true;
}