
//
// Created by sheyme on 03/04/25.
//

#ifndef SWIFTMESSAGE_SRC_SCREEN_HANDLER_INTERFACE_HPP_
#define SWIFTMESSAGE_SRC_SCREEN_HANDLER_INTERFACE_HPP_

#include "greeting_screen.hpp"
#include "sign_screen.hpp"
#include "chat_screen.hpp"

using Result = SignScreen::Result;

struct UserData {
  std::string username;
  std::string login;
  std::string password;
};

class Interface {
 public:
  Interface();

  static Result RenderGreeting();

  Result RenderAR(bool is_registration = false, const std::string& status = "",
                  ColorPairs color = ACTIVE_PAIR);
  [[nodiscard]] UserData GetUserData() const { return user_data_; }

  void RenderChat();
  void RenderChat(std::vector<Message>&& messages);
  void RenderChat(const std::vector<Message>& messages);
  void UpdateMessages(std::vector<Message>&& messages);
  void UpdateMessages(const std::vector<Message>& messages);

  void AddMessagesUpdate(const std::vector<Message>& messages);

  void DisplayAnnouncement(const std::string& message);
  void DisplayError(const std::string& message);
  void DisplayMessage(const std::string& sender, const std::string& message);

  std::string GetSenderLogin();
  std::string GetInputMessage();

  void ClearChat();
  ~Interface();

 private:
  static void init_ncurses();

  static void setup_colors();

  UserData user_data_;
  SignScreen* sign_screen_{nullptr};
  ChatScreen* chat_screen_{nullptr};
  static bool ncurses_initialized;
};

#endif //SWIFTMESSAGE_SRC_SCREEN_HANDLER_INTERFACE_HPP_
