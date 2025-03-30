#include <atomic>
#include <string>
#include <thread>

#include "connection_manager.hpp"

class Client {
 public:
  Client(const std::string& server_ip, const std::string& server_port);

  ~Client();

  bool Connect();

  void Disconnect();

  void SendMessage(const std::string& id, const std::string& message);

  void StartMessage(const std::string& id);

  void Receive();

  void StartReceive();

 private:
  std::string username_;

  std::string server_ip_;
  std::string server_port_;

  ClientConnectionManager connection_;

  std::atomic<bool> is_running_;

  std::thread receiver_thread_;
};