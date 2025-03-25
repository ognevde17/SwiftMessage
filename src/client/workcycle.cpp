#include "Client.hpp"


int main() {
    std::string server_ip = "server_ip";
    std::string server_port = "server_port";
    std::string receiver_ip = "ip";
    Client client(server_ip, server_port);
    client.Connect();
    client.StartReceive();
    client.StartMessage(receiver_ip);
}
