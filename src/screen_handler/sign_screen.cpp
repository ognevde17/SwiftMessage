//
// Created by sheyme on 24/04/25.
//

#include "../../include/screen_handler/sign_screen.hpp"

SignScreen::SignScreen() : AbstractScreen() {
  post_create();
  refresh();
}

void SignScreen::post_create() {
  draw_backgrounds();
  keypad(content_win_, true);
}

void SignScreen::refresh() {
  werase(content_win_);
  werase(main_win_);
  draw_borders();
  draw_fields();
  draw_switcher();
  draw_submit_button();
  AbstractScreen::refresh();
}

SignScreen::Result SignScreen::handle_input() {
  int ch = wgetch(content_win_);
  switch (ch) {
    case (KEY_RESIZE): {
      handle_resize();
      return Result::None;
    }
    case KEY_UP: {
      move_cursor(-1);
      return Result::None;
    }
    case KEY_DOWN: {
      move_cursor(1);
      return Result::None;
    }
    case ('\n') :
    case (KEY_ENTER): {
      return handle_submit();
    }
    case (27) : {
      return Result::Exit;
    }
    case (9): {
      move_cursor(1);
      return Result::None;
    }
    default: {
      handle_char(ch);
      return Result::None;
    }
  }
}

void SignScreen::create_windows() {
  AbstractScreen::create_windows();
}

void SignScreen::handle_resize() {
  delwin(main_win_);
  delwin(content_win_);
  create_windows();
  draw_backgrounds();
  refresh();
}

void SignScreen::handle_char(int ch) {
  if (ch == KEY_UP || ch == KEY_DOWN) {
    return;
  }
  std::string* field = get_current_field();
  if (!field) {
    return;
  }
  if (ch == KEY_BACKSPACE || ch == 127) {
    if (!field->empty()) {
      field->pop_back();
    }
  } else if (isprint(ch)) {
    *field += static_cast<char>(ch);
  }
  refresh();
}

SignScreen::Result SignScreen::handle_submit() {
  if (current_field_ == (is_registration_ ? 4 : 3)) {
    return is_registration_ ? Result::Register : Result::Login;
  } else if (current_field_ == (is_registration_ ? 3 : 2)) {
    is_registration_ = !is_registration_;
    current_field_ = 0;
    clear_fields();
    refresh();
  }
  return Result::None;
}

void SignScreen::move_cursor(int direction) {
  int max_fields = is_registration_ ? 5 : 4;
  current_field_ = (current_field_ + direction + max_fields) % max_fields;
  refresh();
}

std::string* SignScreen::get_current_field() {
  if (is_registration_) {
    switch (current_field_) {
      case (0): return &username_;
      case (1): return &login_;
      case (2): return &password_;
    }
  } else {
    switch (current_field_) {
      case (0): return &login_;
      case (1): return &password_;
    }
  }
  return nullptr;
}

void SignScreen::draw_backgrounds() {
  if (has_colors()) {
    wbkgd(main_win_, COLOR_PAIR(DEFAULT_PAIR));
    wbkgd(content_win_, COLOR_PAIR(DEFAULT_PAIR));
  }
}

void SignScreen::draw_borders() {
  apply_color(main_win_, ACTIVE_PAIR, true);
  box(main_win_, 0, 0);
  apply_color(main_win_, ACTIVE_PAIR, false);
}

void SignScreen::draw_field(const std::string& label, const std::string& value,
                            int x, int y, int field_num) {
  ColorPairs current_color = (field_num == current_field_)
      ? ACTIVE_PAIR : DEFAULT_PAIR;
  apply_color(content_win_, current_color, true);
  mvwprintw(content_win_, y, x, "%s", label.c_str());
  wprintw(content_win_, "[%-20s]", value.c_str());
  apply_color(content_win_, current_color, false);
}

void SignScreen::draw_fields() {
  int x = COLS / 2 - 15;
  int y = LINES / 2 - (is_registration_ ? 3 : 2) - 3;
  if (is_registration_) {
    draw_field("Username:", username_, x, y, 0);
    draw_field("Login:   ", login_, x, y + 2, 1);
    draw_field("Password:", password_, x, y + 4, 2);
  } else {
    draw_field("Login:   ", login_, x, y, 0);
    draw_field("Password:", password_, x, y + 2, 1);
  }
}

void SignScreen::draw_switcher() {
  int x = COLS / 2 - 4;
  int y = LINES / 2 - (is_registration_ ? 0 : 1);
  ColorPairs current_color = current_field_ == (is_registration_ ? 3 : 2)
      ? ACTIVE_PAIR : DEFAULT_PAIR;
  apply_color(content_win_, current_color, true);
  if (is_registration_) {
    mvwprintw(content_win_, y, x, "Sign In");
  } else {
    mvwprintw(content_win_, y, x, "Sign Up");
  }
  apply_color(content_win_, current_color, false);
}

void SignScreen::draw_submit_button() {
  int x = COLS / 2;
  int y = LINES / 2 - (is_registration_ ? -3 : -1);
  ColorPairs current_color = current_field_ == (is_registration_ ? 4 : 3)
      ? ACTIVE_PAIR : DEFAULT_PAIR;
  draw_button(content_win_, x, y, current_color, " Submit ");
}

void SignScreen::clear_fields() {
  username_.clear();
  login_.clear();
  password_.clear();
}
