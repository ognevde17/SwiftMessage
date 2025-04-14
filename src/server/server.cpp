#include "../../include/server/server.hpp"

Server& Server::GetInstance() {
  static Server instance;
  return instance;
}

Server::Server() : io_context_(), connection_manager_() {}

void Server::Run() {
  while (true) {
    int user_id = connection_manager_.AcceptNewClient();
    std::cout << "User " << user_id << " connected" << std::endl;
    std::thread(&Server::Session, this, user_id).detach();
  }
}

void Server::Session(const int user_id) {
  while (true) {
    std::string request = connection_manager_.ReceiveData(user_id);
    RequestManager::HandleRequest(request, user_id);
  }
}
