#include "../../include/client/client.hpp"

int main() {
  std::string server_ip = "192.168.31.187";
  std::string server_port = "1234";
  std::string login;
  std::string login_to;
  
  Client client(server_ip, server_port);
  while (!client.Authenticate()) {
    continue;
  }
  std::cout << "vvedite login polychatelya" << std::endl;
  std::getline(std::cin, login_to);
  client.Connect();
  client.StartReceive();
  login = client.user_login;
  client.StartMessage(login, login_to);
}