
//
// Created by sheyme on 23/04/25.
//

#ifndef SWIFTMESSAGE_SRC_SCREEN_HANDLER_SIGN_SCREEN_HPP_
#define SWIFTMESSAGE_SRC_SCREEN_HANDLER_SIGN_SCREEN_HPP_

#include "abstract_screen.hpp"

using Result = AbstractScreen::Result;

class SignScreen : public AbstractScreen {
 public:
  SignScreen(bool is_registration = false, const std::string& status = "",
             ColorPairs color = ACTIVE_PAIR);

  std::string get_username() const { return username_; }
  std::string get_login() const { return login_; }
  std::string get_password() const { return password_; }

  void refresh() override;

  void update_screen(bool registration_state, const std::string& status = "",
                     ColorPairs color = ACTIVE_PAIR);

  Result handle_input();

 protected:
  void create_windows() override;

  void post_create() override;

 private:
  void handle_resize();

  void handle_char(int ch);

  Result handle_submit();

  void move_cursor(int direction);

  void switch_screen();

  std::string* get_current_field();

  void draw_backgrounds();

  void draw_borders();

  void swap_status();
  void draw_status();

  void draw_field(const std::string& label, const std::string& value,
                  int x, int y, int field_num);

  void draw_fields();

  void draw_switcher();

  void draw_submit_button();

  void clear_fields();

  bool is_registration_{false};
  int current_field_{0};

  std::string status_message_{"Authentication"};
  ColorPairs status_color_{ACTIVE_PAIR};

  std::string username_;
  std::string login_;
  std::string password_;
};

#endif //SWIFTMESSAGE_SRC_SCREEN_HANDLER_SIGN_SCREEN_HPP_