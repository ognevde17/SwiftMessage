//
// Created by sheyme on 24/04/25.
//

#include "../../include/screen_handler/interface.hpp"

Interface::Interface() {
  init_ncurses();
  setup_colors();
}

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

//template <typename... Args>
void Interface::RenderChat() {
  chat_screen_ = new ChatScreen();
//  if constexpr (sizeof... (messages) > 0) {
//    chat_screen_->update_messages(std::forward<Args>(messages)...);
//  }
  chat_screen_->refresh();
}

void Interface::UpdateMessages(const std::vector<Message>& messages) {
  chat_screen_->update_messages(messages);
  chat_screen_->refresh();
}

void Interface::DisplayAnnouncement(const std::string& message) {
  chat_screen_->add_message("[INFO] " + message, SYSTEM_NOTIFICATION_PAIR);
}

void Interface::DisplayError(const std::string& message) {
  chat_screen_->add_message("[ERROR] " + message, SYSTEM_NOTIFICATION_PAIR);
}

void Interface::DisplayMessage(const std::string& sender,
                               const std::string& message) {
  chat_screen_->add_message(sender + ": " + message, ACTIVE_PAIR);
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
  endwin();
}

void Interface::init_ncurses() {
  if (!ncurses_initialized_) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    if (has_colors()) {
      start_color();
      use_default_colors();
    }
    ncurses_initialized_ = true;
  }
}

void Interface::setup_colors() {
  if (has_colors()) {
    init_pair(DEFAULT_PAIR, COLOR_WHITE, COLOR_BLACK);
    init_pair(ACTIVE_PAIR, COLOR_CYAN, COLOR_BLACK);
    init_pair(SENDER_PAIR, COLOR_BLUE, COLOR_BLACK);
    init_pair(SYSTEM_NOTIFICATION_PAIR, COLOR_RED, COLOR_BLACK);
  }
}