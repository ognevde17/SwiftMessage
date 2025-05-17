//
// Created by sheyme on 17/05/25.
//

#ifndef SWIFTMESSAGE_INCLUDE_SCREEN_HANDLER_SEND_SCREEN_HPP_
#define SWIFTMESSAGE_INCLUDE_SCREEN_HANDLER_SEND_SCREEN_HPP_

#include "abstract_screen.hpp"

class SendScreen : public AbstractScreen {
 public:
  SendScreen();

  std::string get_sender_name() const;
  void clear();

  void refresh() override;
  Result handle_input();

 private:
  void handle_resize();

  void draw_border();
  void draw_backgrounds();
  void draw_prompt();
  void draw_input_field();

  std::string current_input_;
};

#endif  // SWIFTMESSAGE_INCLUDE_SCREEN_HANDLER_SEND_SCREEN_HPP_
