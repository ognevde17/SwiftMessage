//
// Created by sheyme on 24/04/25.
//

#include "../../include/screen_handler/abstract_screen.hpp"

AbstractScreen::AbstractScreen() {
  init_ncurses();
  create_windows();
  post_create();
}

void AbstractScreen::display_message(const std::string &message) {
  if (++row_number > static_cast<size_t>(LINES - 2)) {
    scroll(content_win_);
  }
  wprintw(content_win_, "%s\n", message.c_str());
  wrefresh(content_win_);
}

void AbstractScreen::refresh() {
  wrefresh(main_win_);
  wrefresh(content_win_);
}

void AbstractScreen::create_windows() {
  main_win_ = newwin(LINES, COLS, 0, 0);
  content_win_ = derwin(main_win_, LINES-2, COLS-2, 1, 1);
  scrollok(content_win_, true);
}

void AbstractScreen::post_create() { wrefresh(main_win_); }

AbstractScreen::~AbstractScreen() {
  delwin(main_win_);
  delwin(content_win_);
  endwin();
}

void AbstractScreen::init_ncurses() {
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