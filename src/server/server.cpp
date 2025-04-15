#include "../../include/server/server.hpp"

Server& Server::GetInstance() {
  static Server instance;
  return instance;
}

Server::Server() : io_context_(), connection_manager_(), database_manager_(Constants::DATABASE_CONNECTION_STRING), request_manager_() {}

void Server::Run() {
  while (true) {
    int connection_id = connection_manager_.AcceptNewClient();
    std::thread(&Server::Session, this, connection_id).detach();
  }
}

void Server::Session(const int connection_id) {
  while (true) {
    std::string request = connection_manager_.ReceiveData(connection_id);
    request_manager_.HandleRequest(database_manager_, request, connection_id);
  }
}
