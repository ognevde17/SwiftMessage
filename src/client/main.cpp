#include "../../include/client/client.hpp"

int main() {
  std::string server_ip = "127.0.0.1";
  std::string server_port = "1234";

  std::string login;
  std::string login_to;

  std::cout << "vvedite login" << std::endl;
  std::getline(std::cin, login);
  std::cout << "vvedite login polychatelya" << std::endl;
  std::getline(std::cin, login_to);
  Client client(server_ip, server_port);
  std::cout << login << " " << login_to << std::endl;
  client.Connect();
  client.StartReceive();
  client.StartMessage(login, login_to);
}