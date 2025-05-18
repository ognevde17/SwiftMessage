//
// Created by sheyme on 24/04/25.
//

#ifndef SWIFTMESSAGE_INCLUDE_SCREEN_HANDLER_CHAT_SCREEN_HPP_
#define SWIFTMESSAGE_INCLUDE_SCREEN_HANDLER_CHAT_SCREEN_HPP_

#include <vector>

#include "abstract_screen.hpp"

namespace ChatScreenUI {
struct Message {
  Message() = default;

  Message(std::string&& message, ColorPairs type = DEFAULT_PAIR)
      : text(std::move(message)), type(type) {}

  std::string text;
  ColorPairs type;
};
} // namespace ChatScreenUI

class ChatScreen : public AbstractScreen {
 public:
  enum class Result {
    None,
    NewMessage,
    Scroll,
    Exit,
    SendChoice
  };

  ChatScreen();

  Result handle_input();
  std::string get_current_input();

  void refresh() override;
  void load_messages(std::vector<ChatScreenUI::Message>&& messages);
  void load_messages(const std::vector<ChatScreenUI::Message>& messages);
  void add_message(const std::string& message, ColorPairs type = DEFAULT_PAIR);

  void add_messages_update(const std::vector<ChatScreenUI::Message>& messages);

  void update_status(const std::string& sender);
  void update_username(const std::string& username);

  std::vector<ChatScreenUI::Message> get_chat() const { return messages_; }

  void clear_chat();
  ~ChatScreen() override;

 protected:
  void create_windows() override;
  void post_create() override;

 private:
  static std::string wrap_text(const std::string& text, int width);

  void handle_resize();
  void handle_char(int ch);
  void handle_scroll(int direction);

  void draw_background();
  void draw_contacts();
  void draw_chat();
  void draw_input_field();
  void draw_layout();

  WINDOW* contacts_win_;
  WINDOW* chat_win_;
  WINDOW* input_win_;

  std::string host_;
  std::string port_;
  std::string status_ = "Server";

  int scroll_position_{0};
  std::string username_;

  std::string current_input_;
  std::vector<ChatScreenUI::Message> messages_;
  const int kMaxMessages = 1000;
};

#endif //SWIFTMESSAGE_INCLUDE_SCREEN_HANDLER_CHAT_SCREEN_HPP_
