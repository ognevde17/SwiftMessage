//
// Created by sheyme on 24/04/25.
//

#include "../../include/screen_handler/greeting_screen.hpp"

GreetingScreen::GreetingScreen() : AbstractScreen() {
  curs_set(0);
  post_create();
  draw_ui();
}

bool GreetingScreen::handle_input() {
  nodelay(main_win_, true);
  int ch = wgetch(main_win_);
  if (ch == KEY_RESIZE) {
    handle_resize();
    return false;
  }
  return ch == '\n' || ch == KEY_ENTER || ch == ' ';
}

void GreetingScreen::refresh() {
  AbstractScreen::refresh();
  draw_ui();
}

void GreetingScreen::create_windows() {
  main_win_ = newwin(LINES, COLS, 0, 0);
}

void GreetingScreen::post_create() {
  keypad(main_win_, true);
  wtimeout(main_win_, 100);
  if (has_colors()) {
    wbkgd(main_win_, COLOR_PAIR(ACTIVE_PAIR));
  }
}

void GreetingScreen::handle_resize() {
  wclear(main_win_);
  delwin(main_win_);
  create_windows();
  post_create();
  draw_ui();
}

void GreetingScreen::draw_ui() {
  werase(main_win_);
  box(main_win_, 0, 0);
  int x = COLS / 2;
  int y = LINES / 2;
  std::string greeting = "Hello!";
  wattron(main_win_, COLOR_PAIR(ACTIVE_PAIR));
  mvwprintw(main_win_, y - 4,
            x - static_cast<int>(greeting.length()) / 2,
            "%s", greeting.c_str());
  wattroff(main_win_, COLOR_PAIR(ACTIVE_PAIR));
  draw_button(main_win_, x, y, ACTIVE_PAIR, " Continue ");

  wrefresh(main_win_);
}