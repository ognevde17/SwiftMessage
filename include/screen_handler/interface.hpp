//
// Created by sheyme on 03/04/25.
//

#ifndef SWIFTMESSAGE_SRC_SCREEN_HANDLER_INTERFACE_HPP_
#define SWIFTMESSAGE_SRC_SCREEN_HANDLER_INTERFACE_HPP_

#include "greeting_screen.hpp"
#include "sign_screen.hpp"
#include "chat_screen.hpp"
#include <iostream>
enum class AuthChoice {
  Login,
  Register
};

struct AuthCredentials {
  std::string login;
  std::string password;
};
// Мб можно убрать, енам для описания результата действий пользователя
// по экрану регистрации
using Result = SignScreen::Result;

// Пример структуры для хранения
struct UserData {
  std::string username;
  std::string login;
  std::string password;
};

class Interface {
 public:
  // Дефолт </3
  Interface();

  // Экран приветствия. Он сам закончится тогда, когда пользователь на кнопку нажмет
  static void RenderGreeting();

  // Экран авторизации и регистрации. Result - енам выше:
  // если Result::None, то сидим крутим цикл дальше
  // если Result::Login - логика для логина, ::Register - регистрации
  // ::Exit - просто выход из приложения
  Result RenderAR();
  // Выцепить структуру выше со всеми данными
  [[nodiscard]] UserData GetUserData() const { return user_data_; }

  // Экран чата. Если есть аргумент (вектор с сообщениями), то он сразу их выведет
  // Если нет, то пустой чат
  void RenderChat();
  void RenderChat(std::vector<Message>&& messages);
  void RenderChat(const std::vector<Message>& messages);
  // Прогрузить вектор сообщений и обновить чат
  void UpdateMessages(std::vector<Message>&& messages);
  void UpdateMessages(const std::vector<Message>& messages);

  void DisplayAnnouncement(const std::string& message);
  void ShowError(const std::string& message);
  void ShowMessage(const std::string& sender, const std::string& message);
  // Получение учетных данных для аутентификации
  AuthCredentials GetAuthCredentials();
    
  // Получение учетных данных для регистрации
  AuthCredentials GetRegistrationCredentials();
  
  // Выбор между аутентификацией и регистрацией
  AuthChoice GetAuthChoice();
  std::string GetInput();
  // Получение логина получателя сообщения
  std::string GetRecipientLogin();
  std::string GetSenderLogin();  // TODO(Sheyme): потом сделаю
  // Выцепляет ввод пользователя и выводит его в экран
  std::string GetInputMessage();

  void ClearChat();
  ~Interface();

 private:
  void init_ncurses();

  void setup_colors();

  UserData user_data_;
  ChatScreen* chat_screen_{nullptr};
  static bool ncurses_initialized;
};

#endif //SWIFTMESSAGE_SRC_SCREEN_HANDLER_INTERFACE_HPP_