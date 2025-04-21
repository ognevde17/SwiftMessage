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
    interface_.ShowMessage("Успешное подключение к серверу");
    return true;
  } catch (...) {
    interface_.ShowError("Ошибка подключения к серверу");
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
  interface_.ShowMessage("Отключено от сервера");
}

void Client::SendMessage(const std::string& sender_login,
                        const std::string& rec_login, 
                        const std::string& message) {
  if (!connection_.is_connected) {
    interface_.ShowError("Нет соединения с сервером");
    return;
  }
  
  SendMessageRequest data;
  data.message_text = message;
  data.recipient_login = rec_login;
  data.sender_login = sender_login;
  
  if (!connection_.send(data.to_string())) {
    interface_.ShowError("Ошибка отправки сообщения");
  }
}

void Client::StartMessageLoop(const std::string& sender_login, 
                             const std::string& rec_login) {
  while (is_running_) {
    auto message = interface_.GetInput();
    if (message == "endendend") {
      Disconnect();
      break;
    }
    SendMessage(sender_login, rec_login, message);
  }
}

void Client::Receive() {
  while (is_running_) {
    if (!connection_.is_connected) {
      continue;
    }
    try {
      std::string received = connection_.receive(1024);
      SendMessageRequest data = SendMessageRequest::from_string(received);
      if (!data.message_text.empty()) {
        interface_.DisplayMessage(data.sender_login, data.message_text);
      }
    } catch (const boost::system::system_error& e) {
      if (e.code() == boost::asio::error::eof) {
        interface_.ShowError("Сервер закрыл соединение");
      } else {
        interface_.ShowError(std::string("Ошибка приёма: ") + e.what());
      }
      Disconnect();
      break;
    } catch (...) {
      interface_.ShowError("Неизвестная ошибка приёма");
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
  auto credentials = interface_.GetAuthCredentials();
  try {
    if (connection_.SendAuthRequest(credentials.login, credentials.password)) {
      user_login_ = credentials.login;
      is_auth_ = true;
      interface_.ShowMessage("Аутентификация успешна");
      return true;
    } else {
      interface_.ShowError("Ошибка аутентификации");
      return false;
    }
  } catch (...) {
    interface_.ShowError("Ошибка при аутентификации");
    return false;
  }
}

bool Client::Register() {
  auto credentials = interface_.GetRegistrationCredentials();
  try {
    std::string answer = connection_.SendRegRequest(credentials.login, 
                                                 credentials.password);
    if (answer == "You have successfully registered") {
      user_login_ = credentials.login;
      interface_.ShowMessage("Регистрация успешна");
      return true;
    } else if (answer == "ERROR1") {
      interface_.ShowError("Ошибка: пользователь уже существует");
      return false;
    } else {
      interface_.ShowError("Ошибка регистрации");
      return false;
    }
  } catch (...) {
    interface_.ShowError("Ошибка при регистрации");
    return false;
  }
}

void Client::Run() {
  if (!Connect()) return;

  while (!is_auth_) {
    auto auth_choice = interface_.GetAuthChoice();
    if (auth_choice == AuthChoice::Register) {
      Register();
    } else {
      Authenticate();
    }
  }

  auto recipient = interface_.GetRecipientLogin();
  StartReceive();
  
  interface_.ShowMessage("Начало чата с " + recipient);
  StartMessageLoop(user_login_, recipient);
}