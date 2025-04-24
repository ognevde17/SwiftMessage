//
// Created by sheyme on 24/04/25.
//

#ifndef SWIFTMESSAGE_INCLUDE_SCREEN_HANDLER_CHAT_SCREEN_HPP_
#define SWIFTMESSAGE_INCLUDE_SCREEN_HANDLER_CHAT_SCREEN_HPP_

#include "abstract_screen.hpp"

#include <vector>

struct Message {
  Message() = default;

  Message(std::string&& message, bool is_reply = false)
      : text(std::move(message)), is_reply(is_reply) {}

  std::string text;
  bool is_reply;
};

class ChatScreen : public AbstractScreen {
 public:
  enum class Result {
    None,
    NewMessage,
    Scroll,
    Exit
  };

  ChatScreen();

  Result handle_input();
  std::string get_current_input();

  void refresh() override;
  void update_messages(const std::vector<Message>& messages);
  void update_status(const std::string& status);
  void add_message(const std::string& message, bool is_reply = false);

  void clear_chat();
  ~ChatScreen() override;

 protected:
  void create_windows() override;
  void post_create() override;

 private:
  void init_colors();
  static std::string wrap_text(const std::string& text, int width);

  void handle_resize();
  void handle_char(int ch);
  void handle_scroll(int direction);

  void draw_contacts();
  void draw_chat();
  void draw_input_field();
  void draw_layout();

  WINDOW* contacts_win_;
  WINDOW* chat_win_;
  WINDOW* input_win_;
  std::string host_;
  std::string port_;
  std::string status_;
  std::string current_input_;
  std::vector<Message> messages_;
  int scroll_position_{0};
  const int kMaxMessages = 1000;
};

#endif //SWIFTMESSAGE_INCLUDE_SCREEN_HANDLER_CHAT_SCREEN_HPP_
