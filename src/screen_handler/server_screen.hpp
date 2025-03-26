//
// Created by sheyme on 18/03/25.
//

#ifndef SWIFTMESSAGE_SRC_SCREEN_HANDLER_SERVER_SCREEN_HPP_
#define SWIFTMESSAGE_SRC_SCREEN_HANDLER_SERVER_SCREEN_HPP_

#include "abstract_screen.hpp"

class ServerScreen :  public AbstractScreen {
 protected:
  void create_windows() override {
    AbstractScreen::create_windows();
    box(main_win_, 0, 0);
 }
};

#endif //SWIFTMESSAGE_SRC_SCREEN_HANDLER_SERVER_SCREEN_HPP_
