#include "../../include/client/client.hpp"

int main() {
  std::string server_ip = "193.124.117.165";
  std::string server_port = "1234";
  Client client(server_ip, server_port);
  client.Run();
}