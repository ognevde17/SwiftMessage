//
// Created by sheyme on 24/04/25.
//

#include "../../include/screen_handler/interface.hpp"

std::string Interface::sender_login = "";

bool Interface::ncurses_initialized = false;
bool Interface::registration_state = false;

Interface::Interface() {
  InitNcurses();
  SetupColors();
}

Result Interface::RenderGreeting() {
  GreetingScreen greeting_screen;
  while (true) {
    auto state = greeting_screen.handle_input();
    if (state != Result::None) {
      registration_state = state == Result::Register;
      return state;
    }
  }
}

void Interface::InitAR() {
  if (sign_screen_ == nullptr) {
    std::string status = registration_state ? "Registration" : "Authentication";
    sign_screen_ = new SignScreen(registration_state, status, ACTIVE_PAIR);
  }
}

void Interface::SetARStatus(std::string status, ColorPairs color) {
  if (status.empty()) {
    status = registration_state ? "Registration" : "Authentication";
  }
  sign_screen_->update_screen(registration_state, status, color);
}

Result Interface::RenderAR(const std::string& status, ColorPairs color) {
  InitAR();
  SetARStatus(status, color);
  bool is_submitted = false;
  Result state;
  while (!is_submitted) {
    state = sign_screen_->handle_input();
    switch (state) {
      case Result::Login:
        is_submitted = true;
        break;
      case Result::Register:
        registration_state = false;
        is_submitted = true;
        break;
      case Result::Exit:
        is_submitted = true;
        break;
      case Result::None:
        break;
    }
    sign_screen_->refresh();
  }
  user_data_ = UserData{sign_screen_->get_username(), sign_screen_->get_login(),
                        sign_screen_->get_password()};
  return state;
}

void Interface::StopChatLoop() {
  // if (chat_screen_ != nullptr) {
  //   chat_backup_ = chat_screen_->get_chat();
  // }
  // delete chat_screen_;
  if (chat_screen_) {
    delete chat_screen_;
    chat_screen_ = nullptr;
  }
}

void Interface::ResumeChatLoop() {
  // RenderChat();
  // chat_screen_->add_messages_update(std::move(chat_backup_));

  RenderChat(); 

  auto it = chat_history_.find( sender_login );
  if (it != chat_history_.end()) {
    for (const auto& msg : it->second) {
      chat_screen_->add_message(msg.text, ACTIVE_PAIR);
    }
  }

  chat_screen_->refresh();
}

std::string Interface::RenderSendGetter(bool was_running) {
  if (was_running) {
    StopChatLoop();
  }
  SendScreen send_screen;
  while (true) {
    auto state = send_screen.handle_input();
    if (state == Result::Login) {
      sender_login = send_screen.get_sender_name();
      if (was_running) {
        ResumeChatLoop();
      }
      return sender_login;
    }
  }
}

void Interface::RenderChat() {
  chat_screen_ = new ChatScreen();
  chat_screen_->update_status(sender_login);
  chat_screen_->update_username(user_data_.login);
  chat_screen_->refresh();
}

void Interface::RenderChat(std::vector<ChatScreenUI::Message>&& messages) {
  RenderChat();
  UpdateMessages(std::move(messages));
}

void Interface::RenderChat(const std::vector<ChatScreenUI::Message>& messages) {
  RenderChat();
  UpdateMessages(messages);
}

void Interface::UpdateMessages(std::vector<ChatScreenUI::Message>&& messages) {
  chat_screen_->load_messages(
      std::forward<std::vector<ChatScreenUI::Message>>(messages));
  chat_screen_->refresh();
}

void Interface::UpdateMessages(
    const std::vector<ChatScreenUI::Message>& messages) {
  chat_screen_->load_messages(messages);
  chat_screen_->refresh();
}

void Interface::AddMessagesUpdate(
    const std::vector<ChatScreenUI::Message>& messages) {
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
  // chat_screen_->add_message(sender + ": " + message, ACTIVE_PAIR);
  ChatScreenUI::Message msg{sender + ": " + message, ACTIVE_PAIR};
  chat_history_[sender].push_back(msg);

  if (sender == Interface::sender_login) {
    chat_screen_->add_message(msg.text, ACTIVE_PAIR);
  }
}

std::string Interface::GetInputMessage() {
  while (true) {
    auto state = chat_screen_->handle_input();
    if (state == ChatScreen::Result::NewMessage) {
      // auto input = chat_screen_->get_current_input();
      // chat_screen_->add_message(input);
      // return input;
      auto input = chat_screen_->get_current_input();

      ChatScreenUI::Message msg{"You: " + input,
                                SENDER_PAIR};
      chat_history_[Interface::sender_login].push_back(msg);

      chat_screen_->add_message(msg.text);
      chat_screen_->refresh();
      return input;
    }
    if (state == ChatScreen::Result::SendChoice) {
      return "[[SEND]]";
    }
  }
}

void Interface::AddIncomingMessage(const std::string& from, const std::string& text) {
  ChatScreenUI::Message msg{ from + ": " + text, ACTIVE_PAIR };

  chat_history_[from].push_back(msg);

  if (from == sender_login && chat_screen_) {
    chat_screen_->add_message(msg.text, ACTIVE_PAIR);
    chat_screen_->refresh();
  }
}

void Interface::ClearChat() { chat_screen_->clear_chat(); }

Interface::~Interface() {
  delete sign_screen_;
  delete chat_screen_;
  endwin();
}

void Interface::InitNcurses() {
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

void Interface::SetupColors() {
  if (has_colors()) {
    init_pair(DEFAULT_PAIR, COLOR_WHITE, COLOR_BLACK);
    init_pair(ACTIVE_PAIR, COLOR_CYAN, COLOR_BLACK);
    init_pair(SENDER_PAIR, COLOR_BLUE, COLOR_BLACK);
    init_pair(SYSTEM_NOTIFICATION_PAIR, COLOR_RED, COLOR_BLACK);
  }
}
