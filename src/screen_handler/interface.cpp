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

Result Interface::RenderAR(bool is_registration) {
  sign_screen_ = new SignScreen(is_registration);
  bool is_submitted = false;
  Result state;
  while (!is_submitted) {
    state = sign_screen_->handle_input();
    switch (state) {
      case Result::Login:
        is_submitted = true;
        break;
      case Result::Register:
        is_submitted = true;
        break;
      case Result::Exit:
        is_submitted = true;  // Just exit
        break;
      case Result::None:
        break;
    }
    sign_screen_->refresh();
  }
  user_data_ = UserData{sign_screen_->get_username(),
                        sign_screen_->get_login(),
                        sign_screen_->get_password()};
  return state;
}

void Interface::SetARScreenStatus(const std::string& status, ColorPairs color) {
  sign_screen_->set_status(status, color);
  sign_screen_->refresh();
}

void Interface::SwitchARScreen() {
  sign_screen_->switch_screen();
  sign_screen_->set_status("Registration achieved", ACTIVE_PAIR);
}

void Interface::RenderChat() {
  chat_screen_ = new ChatScreen();
  chat_screen_->update_username(user_data_.login);  //TODO(Sheyme): потом сюда юзернейм кидать, а не логин
  chat_screen_->refresh();
}

void Interface::RenderChat(std::vector<Message>&& messages) {
  RenderChat();
  UpdateMessages(std::move(messages));
}

void Interface::RenderChat(const std::vector<Message>& messages) {
  RenderChat();
  UpdateMessages(messages);
}

void Interface::UpdateMessages(std::vector<Message>&& messages) {
  chat_screen_->load_messages(std::forward<std::vector<Message>>(messages));
  chat_screen_->refresh();
}

void Interface::UpdateMessages(const std::vector<Message>& messages) {
  chat_screen_->load_messages(messages);
  chat_screen_->refresh();
}

void Interface::AddMessagesUpdate(const std::vector<Message>& messages) {
  chat_screen_->add_messages_update(messages);
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
  delete sign_screen_;
  delete chat_screen_;
  endwin();
}

void Interface::init_ncurses() {
  if (!ncurses_initialized) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    if (has_colors()) {
      start_color();
      use_default_colors();
    }
    ncurses_initialized = true;
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
