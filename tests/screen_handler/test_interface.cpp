//
// Created by sheyme on 28/04/25.
//

#include <iostream>

#include "../../include/screen_handler/interface.hpp"
#include "utils.hpp"

bool Interface::ncurses_initialized = false;

int main() {
  try {
    Interface interface;

    Interface::RenderGreeting();
    interface.RenderAR();
    auto user_data = interface.GetUserData();

    auto messages = utils::GenerateRandomMessages(testing_data::kMessagesCount);

    interface.RenderChat(std::move(messages));
    interface.DisplayAnnouncement("Logged as: " + user_data.login);
    interface.DisplayAnnouncement("Type exit to quit");

    while (true) {
      auto user_input = interface.GetInputMessage();
      if (user_input == "exit") {
        interface.DisplayAnnouncement("Disconnecting..");
        break;
      }
      if (user_input == "clear") {
        interface.ClearChat();
      }
      if (user_input == "random") {
        messages = utils::GenerateRandomMessages(testing_data::kMessagesCount);
        interface.UpdateMessages(messages);
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