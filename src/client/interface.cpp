#include "../../include/screen_handler/interface.hpp"
#include "../../include/common/messages.hpp"

#include <iostream>
Interface::Interface() {
  init_ncurses();
  setup_colors();
}
bool Interface::ncurses_initialized = false;
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
  chat_screen_->update_messages(std::forward<std::vector<Message>>(messages));
  chat_screen_->refresh();
}

void Interface::UpdateMessages(const std::vector<Message>& messages) {
  chat_screen_->update_messages(messages);
  chat_screen_->refresh();
}

void Interface::DisplayAnnouncement(const std::string& message) {
  chat_screen_->add_message("[INFO] " + message, SYSTEM_NOTIFICATION_PAIR);
}

void Interface::ShowError(const std::string& message) {
  chat_screen_->add_message("[ERROR] " + message, SYSTEM_NOTIFICATION_PAIR);
}

void Interface::ShowMessage(const std::string& sender,
                               const std::string& message) {
  chat_screen_->add_message(sender + ": " + message, ACTIVE_PAIR);
}

std::string Interface::GetInputMessage() {
  while (true) {
    //std::cout << "OK1.1\n";
    if (chat_screen_->handle_input() == ChatScreen::Result::NewMessage) {
        //std::cout << "OK1.2\n";
      auto input = chat_screen_->get_current_input();
      chat_screen_->add_message(input);
      //std::cout << "SUCCess" << std::endl;
      return input;
    }
    //std::cout << "OK1.5\n";
  }
  std::cout << "DDDD" << std::endl;
}
std::string Interface::GetInput() {
    std::string input;
    std::getline(std::cin, input);
    std::cout << "OK1" << std::endl;

    return input;
}

void Interface::ClearChat() { chat_screen_->clear_chat(); }

Interface::~Interface() {
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
AuthCredentials Interface::GetAuthCredentials() {
    AuthCredentials credentials;
    
    std::cout << "Введите логин: ";
    std::getline(std::cin, credentials.login);
    
    std::cout << "Введите пароль: ";
    std::getline(std::cin, credentials.password);
    
    return credentials;
}

AuthCredentials Interface::GetRegistrationCredentials() {
    return GetAuthCredentials(); // В данном случае логика аналогична
}

AuthChoice Interface::GetAuthChoice() {
    while (true) {
        std::cout << "Выберите действие:\n"
                  << "1. Вход\n"
                  << "2. Регистрация\n"
                  << "Ваш выбор: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") return AuthChoice::Login;
        if (choice == "2") return AuthChoice::Register;
        
        ShowError("Некорректный выбор. Попробуйте снова.");
    }
}

std::string Interface::GetRecipientLogin() {
    std::cout << "Введите логин получателя: ";
    return GetInput();
}

