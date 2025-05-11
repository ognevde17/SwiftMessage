//
// Created by sheyme on 26/03/25.
//

#ifndef SWIFTMESSAGE_SRC_SCREEN_HANDLER_ABSTRACT_SCREEN_HPP_
#define SWIFTMESSAGE_SRC_SCREEN_HANDLER_ABSTRACT_SCREEN_HPP_

#pragma once

#include <ncurses.h>
#include <unordered_map>
#include <string>

enum ColorPairs {
  DEFAULT_PAIR = 1,
  ACTIVE_PAIR,
  SYSTEM_NOTIFICATION_PAIR,
  SENDER_PAIR
};

enum class TextAttribute {
  NORMAL = A_NORMAL,
  BOLD = A_BOLD,
  UNDERLINE = A_UNDERLINE,
  REVERSE = A_REVERSE,
  BLINK = A_BLINK
};

struct Format {
  ColorPairs color_pair;
  TextAttribute format_attribute;
};

class AbstractScreen {
 public:
  enum class Result {
    None,
    Login,
    Register,
    Exit
  };

  void display_message(const std::string& message);

  virtual void refresh();

 protected:
  AbstractScreen();

  virtual void create_windows();

  virtual void post_create();

  virtual Format get_format(const ColorPairs& pair);

  virtual void apply_color(WINDOW* win, const ColorPairs& pair, bool state);

  virtual void draw_button(WINDOW* win, int x, int y, ColorPairs color,
                           const std::string& label);

  virtual ~AbstractScreen();

  WINDOW* main_win_;
  WINDOW* content_win_;
  size_t row_number{0};
};

#endif //SWIFTMESSAGE_SRC_SCREEN_HANDLER_ABSTRACT_SCREEN_HPP_
