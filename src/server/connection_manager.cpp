#include "../../include/server/connection_manager.hpp"

std::mutex ConnectionManager::connection_id_to_socket_mutex_;

std::unordered_map<int, tcp::socket>
    ConnectionManager::connection_id_to_socket_;

ConnectionManager::ConnectionManager()
    : acceptor_(io, tcp::endpoint(boost::asio::ip::make_address("0.0.0.0"),
                                  Constants::SERVER_PORT_UNSIGNED_SHORT)) {}

int ConnectionManager::AcceptNewClient() {
  tcp::socket socket = acceptor_.accept();

  int connection_id = GenerateConnectionId();

  AssociateConnectionIdWithSocket(connection_id, std::move(socket));

  return connection_id;
}

int ConnectionManager::GenerateConnectionId() {
  return connection_id_counter_.fetch_add(1);
}

void ConnectionManager::CloseConnection(tcp::socket& socket) { socket.close(); }

// Получение данных

std::string ConnectionManager::ReceiveData(int connection_id) {
  std::unique_lock<std::mutex> lk(connection_id_to_socket_mutex_);

  std::cout << "OK1" << std::endl;
  auto& sock = connection_id_to_socket_.at(connection_id);
  std::cout << "OK2" << std::endl;
  
  lk.unlock();

  return ReceiveData(sock);
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
  std::unique_lock<std::mutex> lk(connection_id_to_socket_mutex_);

  auto& sock = connection_id_to_socket_.at(connection_id);

  lk.unlock();

  return SendData(sock, data);
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
  std::lock_guard<std::mutex> lock(connection_id_to_socket_mutex_);
  ConnectionManager::connection_id_to_socket_.emplace(connection_id,
                                                      std::move(socket));
}

void ConnectionManager::RemoveAssociationBetweenConnectionIdAndSocket(
    int connection_id) {
  std::lock_guard<std::mutex> lock(connection_id_to_socket_mutex_);
  connection_id_to_socket_.erase(connection_id);
}