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
  Interface() = default;

  static void RenderGreeting();

  Result RenderAR();
  [[nodiscard]] UserData GetUserData() const { return user_data_; }

  template <typename... Args>
  void RenderChat(Args&&... messages);
  void UpdateMessages(const std::vector<Message>& messages);

  void DisplayAnnouncement(const std::string& message);
  void DisplayError(const std::string& message);
  void DisplayMessage(const std::string& sender, const std::string& message);

  std::string GetSenderLogin();  // TODO(Sheyme): потом сделаю
  std::string GetInputMessage();

  void ClearChat();
  ~Interface();

 private:
  UserData user_data_;
  ChatScreen* chat_screen_{nullptr};
};

#endif //SWIFTMESSAGE_SRC_SCREEN_HANDLER_INTERFACE_HPP_
