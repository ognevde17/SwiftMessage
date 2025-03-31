#include "../../include/client/client.hpp"
#include "../../include/common/messages.hpp"

Client::Client(const std::string& server_ip, const std::string& server_port)
    : server_ip_(server_ip),
      server_port_(server_port),
      connection_(server_ip, server_port),
      is_running_(false) {}

Client::~Client() { Disconnect(); }

bool Client::Connect() {
  try {
    connection_.Connect();
    return true;
  } catch (...) {
    return false;
  }
}

void Client::Disconnect() {
  is_running_ = false;
  receiver_thread_.detach();
  connection_.Disconnect();
  return;
}

void Client::SendMessage(const std::string& sender_login,
                         const std::string& rec_login,
                         const std::string& message) {
  SendMessageRequest data;
  data.message_text = message;
  data.recipient_login = rec_login;
  data.sender_login = sender_login;
  if (!connection_.send(data.to_string())) {
    std::cerr << "sended failed\n";
  }
}

void Client::StartMessage(const std::string& sender_login,
                          const std::string& rec_login) {
  std::string message;
  // std::cin >> message;
  std::getline(std::cin, message);
  while (message != "endendend") {
    SendMessage(sender_login, rec_login, message);
    // std::cin >> message;
    std::getline(std::cin, message);
  }
  Disconnect();
}

void Client::Receive() {
  while (is_running_) {
    if (!connection_.is_connected) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      continue;
    }

    try {
      std::string received = connection_.receive(1024);
      SendMessageRequest data = SendMessageRequest::from_string(received);
      if (!data.message_text.empty()) {
        std::cout << data.message_text << std::endl;
      }
    } catch (...) {
      if (is_running_) {
        std::cerr << "error in receiving\n";
      }
      throw;
    }
  }
}

void Client::StartReceive() {
  is_running_ = true;
  receiver_thread_ = std::thread(&Client::Receive, this);
}