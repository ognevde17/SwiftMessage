//
// Created by sheyme on 03/04/25.
//

#ifndef SWIFTMESSAGE_SRC_SCREEN_HANDLER_GREETING_SCREEN_HPP_
#define SWIFTMESSAGE_SRC_SCREEN_HANDLER_GREETING_SCREEN_HPP_

#include "abstract_screen.hpp"

class GreetingScreen : public AbstractScreen {
 public:
  GreetingScreen();

  Result handle_input();

  void refresh() override;

 protected:
  void create_windows() override;

  void post_create() override;

 private:
  void handle_resize();

  void draw_ui();

  int current_field_{0};
};

#endif //SWIFTMESSAGE_SRC_SCREEN_HANDLER_GREETING_SCREEN_HPP_
