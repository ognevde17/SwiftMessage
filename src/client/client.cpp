#include "../../include/client/client.hpp"
#include "../../include/common/messages.hpp"

Client::Client(const std::string& server_ip, const std::string& server_port)
    : server_ip_(server_ip),
      server_port_(server_port),
      connection_(server_ip, server_port),
      is_running_(false),
      interface_() {}  // Инициализация интерфейса

Client::~Client() { Disconnect(); }

bool Client::Connect() {
  try {
    connection_.Connect();
    //std::cout << "CONNECTION" << std::endl;
    //interface_.DisplayAnnouncement("Успешное подключение к серверу");
    //std::cout << "FUCK" << std::endl;
    return true;
  } catch (...) {
    //interface_.ShowError("Ошибка подключения к серверу");
    return false;
  }
}

void Client::Disconnect() {
  is_running_ = false;
  is_auth_ = false;
  if (receiver_thread_.joinable()) {
    receiver_thread_.join();
  }
  connection_.Disconnect();
  //interface_.DisplayAnnouncement("Отключено от сервера");
}

void Client::SendMessage(const std::string& sender_login,
                        const std::string& rec_login, 
                        const std::string& message) {
  if (!connection_.is_connected) {
    //interface_.ShowError("Нет соединения с сервером");
    return;
  }
  
  SendMessageRequest data;
  data.message_text = message;
  data.recipient_login = rec_login;
  data.sender_login = sender_login;
  
  if (!connection_.send(data.to_string())) {
    //interface_.ShowError("Ошибка отправки сообщения");
  }
}

void Client::StartMessageLoop(const std::string& sender_login, 
                             const std::string& rec_login) {
  is_running_ = true;
  while (is_running_) {
    auto message = interface_.GetInputMessage();
    if (message == "endendend") {
      Disconnect();
      break;
    }
    //std::cout << "DOSENDMES\n";
    SendMessage(sender_login, rec_login, message);
    //std::cout << "POSLESENDMES\n";
  }
}

void Client::Receive() {
  while (is_running_) {
    if (!connection_.is_connected) {
      continue;
    }
    try {
      //std::cout << "OK1\n";
      std::string received = connection_.receive(1024);
      //std::cout << "OK2\n";
      SendMessageRequest data = SendMessageRequest::from_string(received);
      if (!data.message_text.empty()) {
        interface_.DisplayMessage(data.sender_login, data.message_text);
      }
      //std::cout << "OK3\n";
    } catch (const boost::system::system_error& e) {
      if (e.code() == boost::asio::error::eof) {
        //interface_.ShowError("Сервер закрыл соединение");
      } else {
        //interface_.ShowError(std::string("Ошибка приёма: ") + e.what());
      }
      Disconnect();
      break;
    } catch (...) {
      //interface_.ShowError("Неизвестная ошибка приёма");
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
  std::cout << "auth\n";
  auto data = interface_.GetUserData();
  try {
    std::cout << "t1\n";
    if (connection_.SendAuthRequest(data.login, data.password)) {
      std::cout << "t2\n";
      user_login_ = data.login;
      is_auth_ = true;
      std::cout << "SUCCESS" << std::endl;
      //interface_.DisplayAnnouncement("Аутентификация успешна");
      return true;
    } else {
      std::cout << "FAIL\n";
      //interface_.ShowError("Ошибка аутентификации");
      return false;
    }
  } catch (...) {
    std::cout << "FAIL2\n";
    //interface_.ShowError("Ошибка при аутентификации");
    return false;
  }
}

bool Client::Register() {
  //interface_.RenderAR();
  std::cout << "REG\n";
  auto data = interface_.GetUserData();
  try {
    std::string answer = connection_.SendRegRequest(data.login, 
                                                 data.password);
    if (answer == "You have successfully registered") {
      user_login_ = data.login;
      //interface_.DisplayAnnouncement("Регистрация успешна");
      return true;
    } else if (answer == "ERROR1") {
      //interface_.ShowError("Ошибка: пользователь уже существует");
      return false;
    } else {
      //interface_.ShowError("Ошибка регистрации");
      return false;
    }
  } catch (...) {
    //interface_.ShowError("Ошибка при регистрации");
    return false;
  }
}

void Client::Run() {
  if (!Connect()) return;
  Result state = Result::None;
  bool is_registration = false;
  std::cout << "XYI\n";
  Interface::RenderGreeting();
  std::string status;
  ColorPairs color = ACTIVE_PAIR;
  
  while (!is_auth_) {
    auto state = interface_.RenderAR(status, color);
    if (state == Result::Register) {
      std::cerr << "OK1111\n";
      if (Register()) {
        status = "Registration achieved";
      } else {
        status = "Registration failed";
        color = SYSTEM_NOTIFICATION_PAIR;
      }
      continue;
    } else if (state == Result::Login){
      std::cout << "OK22222\n";
      if (!Authenticate()) {
        status = "Authenticate failed";
        color = SYSTEM_NOTIFICATION_PAIR;
      }
      continue;

    }
    status.clear();
    
  }
    
    

  auto recipient = interface_.GetRecipientLogin();
  interface_.RenderChat();
  StartReceive();
  
  //interface_.DisplayAnnouncement("Начало чата с " + recipient);
  StartMessageLoop(user_login_, recipient);
}