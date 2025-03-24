#include <boost/asio.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
using boost::asio::ip::tcp;
#include <vector>

class ClientConnectionManager {
  public:
    ClientConnectionManager(std::string ip, const std::string& port) : io_context(), socket(io_context), ip(ip), port(port), is_connected(false) {}
    
    bool Connect() {
        try {
            tcp::resolver resolver(io_context);
            boost::asio::connect(socket, resolver.resolve(ip, port));
            is_connected = true;
            return true;
        } catch (...) {
            is_connected = false;
            return false;
        }
    }
    void Disconnect() {
        socket.close();
        is_connected = false;
    }
    bool send(const std::string& message) {
        try {
            boost::asio::write(socket, boost::asio::buffer(message));
            return true;
        } catch (...) {
            std::cerr << "send fail\n";
            return false;
        }
    }
    std::string receive(size_t length) {
        try {
            std::vector<char> reply(length);
            size_t reply_length = socket.read_some(boost::asio::buffer(reply));
            return std::string(reply.data(), reply_length);
        } catch (std::exception& e) {
            std::cerr << "receive fail\n";
            throw;
        }
    }
  private:
    boost::asio::io_context io_context;
    tcp::socket socket;
    std::string ip;
    std::string port;
    bool is_connected;
};
