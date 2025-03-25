#include "ClientConnectionManager.hpp"
#include <string>
#include <atomic>
#include <thread>
class Client {
  public:
    Client(const std::string& server_ip,const std::string& server_port) : server_ip_(server_ip), server_port_(server_port),connection_(server_ip, server_port), is_running_(false) {}
    bool Connect() {
      try {
        connection_.Connect();
        return true;
      } catch (...) {
        return false;
      }
    }
    void Disconnect() {
      is_running_ = false;
      receiver_thread_.detach();
      connection_.Disconnect();
      return;
    }
    void SendMessage(const std::string& id, const std::string& message) {
      if (!connection_.send(id + ":" + message)) {
        std::cerr << "sended failed\n";
      } 
    }
    void StartMessage(const std::string& id) {
      std::string message;
      std::cin >> message;
      while (message != "endendend") {
        SendMessage(id, message);
        std::cin >> message;
      }
    }
    void Receive() {
      while (is_running_) {
        try {
          std::string received = connection_.receive(1024);
          if (!received.empty()) {
            std::cout << received << std::endl;
          }
        } catch (...) {
          if (is_running_) {
            std::cerr << "error in receiving\n";
          }
          throw;
        }
      }
    }
    void StartReceive() {
      is_running_ = true; 
      receiver_thread_ = std::thread(&Receive, this);
    }
  private:
    std::string username_;
    std::string server_ip_;
    std::string server_port_;
    ClientConnectionManager connection_;
    std::atomic<bool> is_running_;
    std::thread receiver_thread_;    
};
