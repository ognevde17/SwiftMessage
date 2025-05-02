#include "../../include/server/server.hpp"

Server& Server::GetInstance() {
  static Server instance;
  return instance;
}

Server::Server()
    : io_context_(),
      connection_manager_(io_context_),
      database_manager_(Constants::DATABASE_CONNECTION_STRING),
      request_manager_() {}

void Server::Run() {
  is_running_.store(true);
  connection_manager_.SetAcceptHandler(
    [this](int connection_id) {
      if (!is_running_.load()) return;
      std::cout << "New client: " << connection_id << std::endl;
      std::thread(&Server::Session, this, connection_id).detach();
    }
  );
  connection_manager_.StartAcceptingClients();

  std::cout << "Server is running (async_accept)..." << std::endl;
  io_context_.run();  // блокирует до acceptor_.close()
}

// void Server::Run() {
//   std::cout << "Server is running" << std::endl;
//   while (true) {
//     std::cout << "Waiting for new client" << std::endl;
//     int connection_id = connection_manager_.AcceptNewClient();
//     std::cout << "New client connected with connection id: " << connection_id
//               << std::endl;
//     std::thread(&Server::Session, this, connection_id).detach();
//   }
// }

// void Server::Stop() {
//   std::cout << "Pushed the <Stop> button" << std::endl;
//   io_context_.stop();
// }

void Server::Stop() {
  std::cout << "Stopping server..." << std::endl;
  is_running_.store(false);
  // безопасно отменяем async_accept внутри io_context
  boost::asio::post(io_context_, [this]() {
    connection_manager_.CloseAcceptor();
  });
}

void Server::Session(const int connection_id) {
  while (true) {
    std::string request = connection_manager_.ReceiveData(connection_id);
    request_manager_.HandleRequest(database_manager_, request, connection_id);
    std::cout << "handle request ended" << std::endl;
  }
}
