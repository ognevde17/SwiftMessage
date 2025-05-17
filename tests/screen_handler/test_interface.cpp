
//
// Created by sheyme on 28/04/25.
//

#include <iostream>

#include "../../include/screen_handler/interface.hpp"
#include "./utils.hpp"

bool Interface::ncurses_initialized = false;
bool Interface::registration_state = false;
std::string Interface::sender_login = "server";

int main() {
  try {
    Interface interface;

    Interface::RenderGreeting();

    std::string status;
    ColorPairs color = ACTIVE_PAIR;
    while (true) {
      Result state = interface.RenderAR(status, color);
      if (state == Result::Register) {
        status = "Registration achieved";
        color = SYSTEM_NOTIFICATION_PAIR;
      }
      if (state == Result::Login || state == Result::Exit) {
        break;
      }
    }

    auto user_data = interface.GetUserData();
    auto messages = utils::GenerateRandomMessages(testing_data::kMessagesCount);
    auto sender = interface.RenderSendGetter();

    interface.RenderChat(std::move(messages));
    interface.DisplayAnnouncement("Logged as: " + user_data.login);
    interface.DisplayAnnouncement("Type exit to quit");
    interface.DisplayAnnouncement("Sender is: " + sender);

    while (true) {
      auto user_input = interface.GetInputMessage();
      if (user_input == "[[SEND]]") {
        sender = interface.RenderSendGetter(true);
        interface.DisplayAnnouncement("Chat swapped to: " + sender);
        continue;
      }
      if (user_input == "exit") {
        interface.DisplayAnnouncement("Disconnecting..");
        break;
      }
      if (user_input == "clear") {
        interface.ClearChat();
        continue;
      }
      if (user_input == "random") {
        messages = utils::GenerateRandomMessages(
            testing_data::kMessagesUpdateCount);
        interface.AddMessagesUpdate(messages);
        continue;
      }
      if (user_input == "sender") {
        interface.DisplayMessage(sender, "Test sender message");
        continue;
      }
      if (user_input == "error") {
        interface.DisplayError("Testing error");
      } else {
        interface.DisplayMessage("Server", std::string(user_input));
      }
    }
  }
  catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}