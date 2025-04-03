//
// Created by sheyme on 03/04/25.
//

#ifndef SWIFTMESSAGE_SRC_SCREEN_HANDLER_INTERFACE_HPP_
#define SWIFTMESSAGE_SRC_SCREEN_HANDLER_INTERFACE_HPP_

#include "greeting_screen.hpp"

class Interface {
 public:
  void Greetings() {
    GreetingScreen greeting_screen_;
    bool is_pressed = false;
    while (!is_pressed) {
      is_pressed = greeting_screen_.handle_input();
    }
  }

 protected:
 private:
};

#endif //SWIFTMESSAGE_SRC_SCREEN_HANDLER_INTERFACE_HPP_
