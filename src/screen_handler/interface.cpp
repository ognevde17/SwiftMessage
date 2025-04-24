//
// Created by sheyme on 24/04/25.
//

#include "../../include/screen_handler/interface.hpp"

void Interface::RenderGreeting() {
  GreetingScreen greeting_screen;
  bool is_pressed = false;
  while (!is_pressed) {
    is_pressed = greeting_screen.handle_input();
  }
}

Result Interface::RenderAR() {
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

template <typename... Args>
void Interface::RenderChat(Args&&... messages) {
  chat_screen_ = new ChatScreen();
  if constexpr (sizeof... (messages) > 0) {
    chat_screen_->update_messages(std::forward<Args>(messages)...);
  }
  chat_screen_->refresh();
}

void Interface::UpdateMessages(const std::vector<Message>& messages) {
  chat_screen_->update_messages(messages);
  chat_screen_->refresh();
}

void Interface::DisplayAnnouncement(const std::string& message) {
  chat_screen_->add_message("[INFO] " + message, true);
}

void Interface::DisplayError(const std::string& message) {
  chat_screen_->add_message("[ERROR] " + message, true);
}

void Interface::DisplayMessage(const std::string& sender,
                               const std::string& message) {
  chat_screen_->add_message(sender + ": " + message, true);
}

std::string Interface::GetInputMessage() {
  while (true) {
    if (chat_screen_->handle_input() == ChatScreen::Result::NewMessage) {
      auto input = chat_screen_->get_current_input();
      chat_screen_->add_message(input);
      return input;
    }
  }
}

void Interface::ClearChat() { chat_screen_->clear_chat(); }

Interface::~Interface() {
  if (chat_screen_ != nullptr) {
    delete chat_screen_;
  }
}