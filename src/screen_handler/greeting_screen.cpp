//
// Created by sheyme on 24/04/25.
//

#include "../../include/screen_handler/greeting_screen.hpp"

using Result = AbstractScreen::Result;

GreetingScreen::GreetingScreen() : AbstractScreen() {
  curs_set(0);
  post_create();
  draw_ui();
}

Result GreetingScreen::handle_input() {
  int ch = wgetch(main_win_);
  switch (ch) {
    case (KEY_RESIZE):
      handle_resize();
    case (KEY_LEFT):
      current_field = 1 - current_field;
      refresh();
      return Result::None;
    case (KEY_RIGHT):
      current_field = 1 - current_field;
      refresh();
      return Result::None;
    case ('\n'):
      return current_field == 0 ? Result::Login : Result::Register;
    case (KEY_ENTER):
      return current_field == 0 ? Result::Login : Result::Register;
    case (27):
      return Result::Exit;
    case (9):
      current_field = 1 - current_field;
      refresh();
      return Result::None;
    default:
      return Result::None;
  }
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
  std::string greeting = "Welcome to SwiftMessage!";
  apply_color(main_win_, ColorPairs(ACTIVE_PAIR), true);
  mvwprintw(main_win_, y - 4,
            x - (static_cast<int>(greeting.length()) / 2),
            "%s", greeting.c_str());
  apply_color(main_win_, ColorPairs(ACTIVE_PAIR), false);
  int button_spacing = 10;
  ColorPairs current_color = current_field == 0 ? ACTIVE_PAIR : DEFAULT_PAIR;
  draw_button(main_win_, x - button_spacing, y, current_color, " Sign In ");
  current_color = current_field == 1 ? ACTIVE_PAIR : DEFAULT_PAIR;
  draw_button(main_win_, x + button_spacing, y, current_color, " Sign Up ");
  wrefresh(main_win_);
}