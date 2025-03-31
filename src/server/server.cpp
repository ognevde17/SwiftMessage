#include "../../include/server/server.hpp"

Server::Server() : io_context_(), connection_manager_() {}

void Server::Run() {
  while (true) {
    int user_id = connection_manager_.AcceptNewClient();
    std::cout << "User " << user_id << " connected" << std::endl;
    std::thread(&Server::Session, this, user_id).detach();
  }
}

void Server::Session(int user_id) {
  while (true) {
    std::string request = connection_manager_.ReceiveData(user_id);
    RequestManager::HandleRequest(request);
  }
}
