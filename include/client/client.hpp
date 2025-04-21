#include <atomic>
#include <string>
#include <thread>

#include "connection_manager.hpp"
#include "interface.hpp"
class Client {
 public:
  Client(const std::string& server_ip, const std::string& server_port);

  ~Client();

  bool Connect();

  void Disconnect();

  void SendMessage(const std::string& sender_login, const std::string& rec_login, const std::string& message);

  void StartMessageLoop(const std::string& sender_login, const std::string& rec_login);

  void Receive();

  void StartReceive();

  bool Authenticate();
  bool Register();
  void Run();
  std::string user_login_ = "";

 private:
  std::string username_;

  std::string server_ip_;
  std::string server_port_;

  ClientConnectionManager connection_;
  std::atomic<bool> is_running_ = false;
  std::atomic<bool> is_auth_ = false;
  std::thread receiver_thread_;
  Interface interface_;
};