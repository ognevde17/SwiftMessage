//
// Created by sheyme on 17/05/25.
//

#include "../../include/screen_handler/send_screen.hpp"

SendScreen::SendScreen() {
  keypad(content_win_, true);
  refresh();
}

void SendScreen::refresh() {
  werase(content_win_);
  draw_border();
  draw_backgrounds();
  draw_prompt();
  draw_input_field();
  AbstractScreen::refresh();
}

std::string SendScreen::get_sender_name() const { return current_input_; }

void SendScreen::clear() {
  current_input_.clear();
  refresh();
}

AbstractScreen::Result SendScreen::handle_input() {
  int ch = wgetch(content_win_);
  switch (ch) {
    case KEY_RESIZE:
      handle_resize();
      return Result::None;
    case '\n':
      if (!current_input_.empty()) {
        return Result::Login;
      }
      return Result::None;
    case KEY_ENTER:
      if (!current_input_.empty()) {
        return Result::Login;
      }
      return Result::None;
    case KEY_BACKSPACE:
      if (!current_input_.empty()) {
        current_input_.pop_back();
      }
      refresh();
      return Result::None;
    case 127:
      if (!current_input_.empty()) {
        current_input_.pop_back();
      }
      refresh();
      return Result::None;
    default:
      if ((isprint(ch) != 0) && current_input_.length() < 30) {
        current_input_ += static_cast<char>(ch);
        refresh();
      }
      return Result::None;
  }
}

void SendScreen::handle_resize() {
  wclear(content_win_);
  wclear(main_win_);
  delwin(content_win_);
  delwin(main_win_);
  create_windows();
  post_create();
}

void SendScreen::draw_border() {
  apply_color(main_win_, ACTIVE_PAIR, true);
  box(main_win_, 0, 0);
  apply_color(main_win_, ACTIVE_PAIR, false);
}

void SendScreen::draw_backgrounds() {
  if (has_colors()) {
    wbkgd(main_win_, COLOR_PAIR(DEFAULT_PAIR));
    wbkgd(content_win_, COLOR_PAIR(DEFAULT_PAIR));
  }
}

void SendScreen::draw_prompt() {
  std::string prompt = "Enter sender name:";
  int x = (COLS / 2) - (prompt.length() / 2);
  int y = (LINES / 2) - 4;
  apply_color(content_win_, DEFAULT_PAIR, true);
  mvwprintw(content_win_, y, x, "%s", prompt.c_str());
  apply_color(content_win_, DEFAULT_PAIR, false);
}

void SendScreen::draw_input_field() {
  int field_width = 20;
  int x = (COLS / 2) - (field_width / 2);
  int y = (LINES / 2) - 2;
  apply_color(content_win_, ACTIVE_PAIR, true);
  mvwhline(content_win_, y, x, ' ', field_width);
  wprintw(content_win_, "[%-20s]", current_input_.c_str());
  apply_color(content_win_, ACTIVE_PAIR, false);
  wmove(content_win_, y, x + current_input_.length());
}