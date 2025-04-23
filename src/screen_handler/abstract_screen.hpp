//
// Created by sheyme on 26/03/25.
//

#ifndef SWIFTMESSAGE_SRC_SCREEN_HANDLER_ABSTRACT_SCREEN_HPP_
#define SWIFTMESSAGE_SRC_SCREEN_HANDLER_ABSTRACT_SCREEN_HPP_

#pragma once

#include <ncurses.h>
#include <string>

enum ColorPairs {
  CONTACTS_PAIR = 1,
  CHAT_PAIR,
  REPLY_PAIR,
  INPUT_PAIR,
  DEFAULT_PAIR,
  ACTIVE_PAIR
};

class AbstractScreen {
 public:
  void display_message(const std::string& message) {
    if (++row_number > static_cast<size_t>(LINES - 2)) {
      scroll(content_win_);
    }
    wprintw(content_win_, "%s\n", message.c_str());
    wrefresh(content_win_);
  }

  virtual void refresh() {
    wrefresh(main_win_);
    wrefresh(content_win_);
  }

 protected:
  AbstractScreen() {
    init_ncurses();
    create_windows();
    post_create();
  }

  virtual void create_windows() {
    main_win_ = newwin(LINES, COLS, 0, 0);
    content_win_ = derwin(main_win_, LINES-2, COLS-2, 1, 1);
    scrollok(content_win_, TRUE);
  }
  virtual void post_create() {
    wrefresh(main_win_);
  }

  virtual ~AbstractScreen() {
    delwin(main_win_);
    delwin(content_win_);
    endwin();
  }

  WINDOW* main_win_;
  WINDOW* content_win_;
  size_t row_number{0};

 private:
  void init_ncurses() {
    if (!ncurses_initialized_) {
      initscr();
      cbreak();
      noecho();
      keypad(stdscr, true);
      if (has_colors()) {
        start_color();
        use_default_colors();
      }
      ncurses_initialized_ = true;
    }
  }

  static bool ncurses_initialized_;
};

bool AbstractScreen::ncurses_initialized_ = false;

#endif //SWIFTMESSAGE_SRC_SCREEN_HANDLER_ABSTRACT_SCREEN_HPP_
