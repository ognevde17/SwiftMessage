//
// Created by sheyme on 03/04/25.
//

#ifndef SWIFTMESSAGE_SRC_SCREEN_HANDLER_INTERFACE_HPP_
#define SWIFTMESSAGE_SRC_SCREEN_HANDLER_INTERFACE_HPP_

#include "greeting_screen.hpp"
#include "sign_screen.hpp"
//#include "chat_screen.hpp"

struct UserData {
  std::string username;
  std::string login;
  std::string password;
};

class Interface {
 public:
  using Result = SignScreen::Result;

  Interface() = default;

  static void RenderGreeting() {
    GreetingScreen greeting_screen;
    bool is_pressed = false;
    while (!is_pressed) {
      is_pressed = greeting_screen.handle_input();
    }
  }

  Result RenderAR() {
    SignScreen sign_screen;
    bool is_submitted = false;
    Result state;
    while (!is_submitted) {
      state = sign_screen.handle_input();
      switch (state) {
        case Result::Login:  // TODO(Anyone): login logic
        case Result::Register:  // TODO(Anyone): reg logic
          is_submitted = true;
          break;
        case Result::Exit:
          is_submitted = true;  // Just exit
          break;
        case Result::None:
          break;
      }
      sign_screen.refresh();
    }
    user_data_ = UserData{sign_screen.get_username(),
                          sign_screen.get_login(),
                          sign_screen.get_password()};
    return state;
  }

  [[nodiscard]] UserData GetUserData() const { return user_data_; }

 private:
  UserData user_data_;
};

#endif //SWIFTMESSAGE_SRC_SCREEN_HANDLER_INTERFACE_HPP_
