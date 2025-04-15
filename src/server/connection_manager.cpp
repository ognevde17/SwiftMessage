#include "../../include/server/connection_manager.hpp"

std::mutex connection_id_to_socket_mutex_;

std::unordered_map<int, tcp::socket>
    ConnectionManager::connection_id_to_socket_;

ConnectionManager::ConnectionManager()
    : acceptor_(io, tcp::endpoint(boost::asio::ip::make_address("0.0.0.0"),
                                  Constants::SERVER_PORT_UNSIGNED_SHORT)) {}

int ConnectionManager::AcceptNewClient() {
  tcp::socket socket = acceptor_.accept();

  std::cout << "New client connected" << std::endl;

  int connection_id = GenerateConnectionId();

  std::cout << "OK1" << std::endl;

  AssociateConnectionIdWithSocket(connection_id, std::move(socket));

  std::cout << "OK2" << std::endl;

  return connection_id;
}

int ConnectionManager::GenerateConnectionId() {
  return connection_id_counter_.fetch_add(1);
}

void ConnectionManager::CloseConnection(tcp::socket& socket) { socket.close(); }

// Получение данных

std::string ConnectionManager::ReceiveData(int connection_id) {
  std::lock_guard<std::mutex> lock(connection_id_to_socket_mutex_);
  return ReceiveData(connection_id_to_socket_.at(connection_id));
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

// Отправка данных

bool ConnectionManager::SendData(int connection_id, const std::string& data) {
  std::lock_guard<std::mutex> lock(connection_id_to_socket_mutex_);
  return SendData(connection_id_to_socket_.at(connection_id), data);
}

bool ConnectionManager::SendData(tcp::socket& socket, const std::string& data) {
  boost::system::error_code error;

  boost::asio::write(socket, boost::asio::buffer(data), error);

  if (error) {
    return false;
  }

  return true;
}

// Добавление и удаление ассоциаций

void ConnectionManager::AssociateConnectionIdWithSocket(int connection_id,
                                                        tcp::socket socket) {
  std::cout << "OK2.5" << std::endl;
  std::lock_guard<std::mutex> lock(connection_id_to_socket_mutex_);
  std::cout << "OK3" << std::endl;
  ConnectionManager::connection_id_to_socket_.emplace(connection_id,
                                                      std::move(socket));
  std::cout << "OK4" << std::endl;
}

void ConnectionManager::RemoveAssociationBetweenConnectionIdAndSocket(
    int connection_id) {
  std::lock_guard<std::mutex> lock(connection_id_to_socket_mutex_);
  connection_id_to_socket_.erase(connection_id);
}