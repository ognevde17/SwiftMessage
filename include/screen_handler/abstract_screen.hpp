//
// Created by sheyme on 26/03/25.
//

#ifndef SWIFTMESSAGE_SRC_SCREEN_HANDLER_ABSTRACT_SCREEN_HPP_
#define SWIFTMESSAGE_SRC_SCREEN_HANDLER_ABSTRACT_SCREEN_HPP_

#pragma once

#include <ncurses.h>
#include <string>

enum ColorPairs {
  DEFAULT_PAIR,
  ACTIVE_PAIR,
  SYSTEM_NOTIFICATION_PAIR,
  SENDER_PAIR
};

class AbstractScreen {
 public:
  void display_message(const std::string& message);

  virtual void refresh();

 protected:
  AbstractScreen();

  virtual void create_windows();

  virtual void post_create();

  virtual ~AbstractScreen();

  WINDOW* main_win_;
  WINDOW* content_win_;
  size_t row_number{0};

 private:
  void init_ncurses();

  static bool ncurses_initialized_;
};

bool AbstractScreen::ncurses_initialized_ = false;

#endif //SWIFTMESSAGE_SRC_SCREEN_HANDLER_ABSTRACT_SCREEN_HPP_
