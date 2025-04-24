//
// Created by sheyme on 03/04/25.
//

#ifndef SWIFTMESSAGE_SRC_SCREEN_HANDLER_INTERFACE_HPP_
#define SWIFTMESSAGE_SRC_SCREEN_HANDLER_INTERFACE_HPP_

#include "greeting_screen.hpp"
#include "sign_screen.hpp"
#include "chat_screen.hpp"

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
  Interface() = default;

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
  template <typename... Args>
  void RenderChat(Args&&... messages);
  // Прогрузить вектор сообщений и обновить чат
  void UpdateMessages(const std::vector<Message>& messages);

  void DisplayAnnouncement(const std::string& message);
  void DisplayError(const std::string& message);
  void DisplayMessage(const std::string& sender, const std::string& message);

  std::string GetSenderLogin();  // TODO(Sheyme): потом сделаю
  // Выцепляет ввод пользователя и выводит его в экран
  std::string GetInputMessage();

  void ClearChat();
  ~Interface();

 private:
  UserData user_data_;
  ChatScreen* chat_screen_{nullptr};
};

#endif //SWIFTMESSAGE_SRC_SCREEN_HANDLER_INTERFACE_HPP_
