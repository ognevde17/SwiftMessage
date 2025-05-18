//
// Created by sheyme on 03/04/25.
//

#ifndef SWIFTMESSAGE_SRC_SCREEN_HANDLER_INTERFACE_HPP_
#define SWIFTMESSAGE_SRC_SCREEN_HANDLER_INTERFACE_HPP_

#include "greeting_screen.hpp"
#include "sign_screen.hpp"
#include "send_screen.hpp"
#include "chat_screen.hpp"
#include <unordered_map>
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
  void RenderChat(std::vector<ChatScreenUI::Message>&& messages);
  void RenderChat(const std::vector<ChatScreenUI::Message>& messages);
  void UpdateMessages(std::vector<ChatScreenUI::Message>&& messages);
  void UpdateMessages(const std::vector<ChatScreenUI::Message>& messages);

  void AddMessagesUpdate(const std::vector<ChatScreenUI::Message>& messages);

  void AddIncomingMessage(const std::string& from, const std::string& text);

  void DisplayAnnouncement(const std::string& message);
  void DisplayError(const std::string& message);
  void DisplayMessage(const std::string& sender, const std::string& message);

  static std::string GetSender() { return sender_login; }
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

  std::unordered_map<std::string, std::vector<ChatScreenUI::Message>> chat_history_;

  static std::string sender_login;
  static bool ncurses_initialized;
  static bool registration_state;

  UserData user_data_;
  SignScreen* sign_screen_{nullptr};
  ChatScreen* chat_screen_{nullptr};
  std::vector<ChatScreenUI::Message> chat_backup_;
};

#endif //SWIFTMESSAGE_SRC_SCREEN_HANDLER_INTERFACE_HPP_
