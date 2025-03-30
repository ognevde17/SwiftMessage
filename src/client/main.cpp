#include "../../include/client/client.hpp"

int main() {
  std::string server_ip = "93.175.4.151";
  std::string server_port = "1234";
  std::string login;
  std::string login_to;
  std::cout << "vvedite login" << std::endl;
  std::cin >> login;
  std::cout << "vvedite login polychatelya" << std::endl;
  std::cin >> login_to;
  Client client(server_ip, server_port);
  
  client.Connect();
  client.StartReceive();
  client.StartMessage(login, login_to);
}