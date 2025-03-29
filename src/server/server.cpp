#include "../../include/server/server.hpp"

Server::Server() : io_context_(), connection_manager_() {}

void Server::Run() {
  while (true) {
    int client_id = connection_manager_.AcceptNewClient();
    Session(client_id);
  }
}

void Server::Session(int user_id) {
  while (true) {
    std::string request = connection_manager_.ReceiveData(user_id);
    std::thread(RequestManager::HandleRequest, request).detach();
  }
}
