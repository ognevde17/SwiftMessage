
//
// Created by sheyme on 03/04/25.
//

#ifndef SWIFTMESSAGE_SRC_SCREEN_HANDLER_INTERFACE_HPP_
#define SWIFTMESSAGE_SRC_SCREEN_HANDLER_INTERFACE_HPP_

#include "greeting_screen.hpp"
#include "sign_screen.hpp"
#include "send_screen.hpp"
#include "chat_screen.hpp"

struct UserData {
  std::string username;
  std::string login;
  std::string password;
};

class Interface {
 public:
  Interface();

  static Result RenderGreeting();

  Result RenderAR(const std::string& status = "",
                  ColorPairs color = ACTIVE_PAIR);
  [[nodiscard]] UserData GetUserData() const { return user_data_; }

  std::string RenderSendGetter(bool was_running = false);

  void RenderChat();
  void RenderChat(std::vector<Message>&& messages);
  void RenderChat(const std::vector<Message>& messages);
  void UpdateMessages(std::vector<Message>&& messages);
  void UpdateMessages(const std::vector<Message>& messages);

  void AddMessagesUpdate(const std::vector<Message>& messages);

  void DisplayAnnouncement(const std::string& message);
  void DisplayError(const std::string& message);
  void DisplayMessage(const std::string& sender, const std::string& message);

  std::string GetInputMessage();

  void ClearChat();
  ~Interface();

 private:
  static void InitNcurses();

  static void SetupColors();

  void InitAR();
  void SetARStatus(std::string status, ColorPairs color);

  void StopChatLoop();
  void ResumeChatLoop();

  UserData user_data_;
  SignScreen* sign_screen_{nullptr};
  ChatScreen* chat_screen_{nullptr};
  static std::string sender_login;
  std::vector<Message> chat_backup_;
  static bool ncurses_initialized;
  static bool registration_state;
};

#endif //SWIFTMESSAGE_SRC_SCREEN_HANDLER_INTERFACE_HPP_
