//
// Created by sheyme
//
#include <iostream>

#include "../../include/screen_handler/interface.hpp"

bool Interface::ncurses_initialized_ = false;

int main() {
  try {
    Interface interface;

    Interface::RenderGreeting();
    interface.RenderAR();
    auto user_data = interface.GetUserData();
//    std::vector<std::string> test_vec;
    interface.RenderChat();
    interface.DisplayAnnouncement("Logged as:" + user_data.login);
    interface.DisplayAnnouncement("Type exit to quit");

    while (true) {
      auto user_input = interface.GetInputMessage();
      if (user_input == "exit") {
        interface.DisplayAnnouncement("Disconnecting..");
        break;
      } else if (user_input == "error") {
        interface.DisplayError("Testing error");
      } else {
        interface.DisplayMessage("Server", std::string(user_input));
      }
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}