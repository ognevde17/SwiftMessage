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
  is_auth = false;
  receiver_thread_.detach();
  connection_.Disconnect();
  return;
}

void Client::SendMessage(const std::string& sender_login,const std::string& rec_login, const std::string& message) {
  if (!connection_.is_connected) {
    std::cerr << "Нет соединения с сервером\n";
    return;
  }
  SendMessageRequest data;
  data.message_text = message;
  data.recipient_login = rec_login;
  data.sender_login = sender_login;
  if (!connection_.send(data.to_string())) {
    std::cerr << "sended failed\n";
  }
  std::cout << "Успешно отправлено: " << message << std::endl;
}

void Client::StartMessage(const std::string& sender_login,
                          const std::string& rec_login) {
  std::string message;
  // std::cin >> message;
  std::getline(std::cin, message);
  std::cout << "message: " << message << std::endl;
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
        std::cout << data.sender_login << ": " << data.message_text << std::endl;
      }
    } catch (const boost::system::system_error& e) {
            if (e.code() == boost::asio::error::eof) {
                std::cerr << "Сервер закрыл соединение\n";
            } else {
                std::cerr << "Ошибка приёма: " << e.what() << "\n";
            }
            Disconnect();
            break;
        } catch (...) {
            std::cerr << "Неизвестная ошибка приёма\n";
            Disconnect();
            break;
        }
  }
}

void Client::StartReceive() {
  is_running_ = true;
  receiver_thread_ = std::thread(&Client::Receive, this);
}
bool Client::Authenticate() {
  std::cout << "vvedite login" << std::endl;
  std::string login;
  std::getline(std::cin, login);
  std::cout << "vvedite password" << std::endl;
  std::string password;
  std::getline(std::cin, password);
  try {
    if (connection_.SendAuthRequest(login, password)) {
      user_login = login;
      is_auth = true;
      return true;
    } else {
      return false;
    }
  } catch (...) {
    return false;
  }
}
bool Client::Register() {
  std::cout << "vvedite login" << std::endl;
  std::string login;
  std::getline(std::cin, login);
  std::cout << "vvedite password" << std::endl;
  std::string password;
  std::getline(std::cin, password);
  try {
    std::string answer = connection_.SendRegRequest(login, password);
    if (answer == "reg_success") {
      user_login = login;
      std::cout << "REGISTRATION SUCCESS" << std::endl;
      return true;
    } else if (answer == "ERROR1") {
      std::cout << "ERROR: This user already exists" << std::endl;
      return false;
    } else {
      std::cout << "ERROR" << std::endl;
      return false;
    }
  } catch (...) {
    std::cout << "ERROR" << std::endl;
    return false;
  }
}

void Client::Run() {
  while (!is_auth) {
    std::cout << "Auth or Register?" << std::endl;
    std::string ans;
    std::getline(std::cin, ans);
    if (ans[0] == 'r' || ans[0] == 'R') {
      Register();
    } else {
      Authenticate();
    }
  }
  std::cout << "vvedite login polychatelya" << std::endl;
  std::string login_to, login;
  std::getline(std::cin, login_to);
  Connect();
  StartReceive();
  login = user_login;
  StartMessage(login, login_to);
}