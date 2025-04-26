//
// Created by sheyme on 24/04/25.
//

#include "../../include/screen_handler/sign_screen.hpp"

SignScreen::SignScreen() : AbstractScreen() {
  post_create();
  refresh();
}

void SignScreen::post_create() {
  wbkgd(main_win_, COLOR_PAIR(DEFAULT_PAIR));
  wbkgd(content_win_, COLOR_PAIR(DEFAULT_PAIR));
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
  wbkgd(main_win_, COLOR_PAIR(DEFAULT_PAIR));
  wbkgd(content_win_, COLOR_PAIR(DEFAULT_PAIR));
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

void SignScreen::draw_field(const std::string &label, const std::string &value,
                            int x, int y, int field_num) {
  bool active = (field_num == current_field_);
  wattron(content_win_, COLOR_PAIR(active ? ACTIVE_PAIR : DEFAULT_PAIR));
  mvwprintw(content_win_, y, x, "%s", label.c_str());
  wprintw(content_win_, "[%-20s]", value.c_str());
  wattroff(content_win_, COLOR_PAIR(active ? ACTIVE_PAIR : DEFAULT_PAIR));
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
  wattron(content_win_, COLOR_PAIR(current_field_ == (is_registration_ ? 3 : 2)
                                       ? ACTIVE_PAIR : DEFAULT_PAIR));
  if (is_registration_) {
    mvwprintw(content_win_, y, x, "Sign In");
  } else {
    mvwprintw(content_win_, y, x, "Sign Up");
  }
  wattroff(content_win_, COLOR_PAIR(current_field_ == (is_registration_ ? 3 : 2)
                                        ? ACTIVE_PAIR : DEFAULT_PAIR));
}

void SignScreen::draw_submit_button() {
  int x = COLS / 2;
  int y = LINES / 2 - (is_registration_ ? -3 : -1);
  bool active = (current_field_ == (is_registration_ ? 4 : 3));
  std::string button_text = " Submit ";
  int button_width = static_cast<int>(button_text.length()) + 2;
  int button_height = 3;
  int button_start_x = x - button_width / 2;
  int button_start_y = y + 1;
  wattron(content_win_, COLOR_PAIR(active ? ACTIVE_PAIR : DEFAULT_PAIR));
  mvwaddch(content_win_, button_start_y, button_start_x, ACS_ULCORNER);
  mvwhline(content_win_, button_start_y, button_start_x + 1,
           ACS_HLINE, button_width - 2);
  mvwaddch(content_win_, button_start_y, button_start_x + button_width - 1,
           ACS_URCORNER);
  for (int idx = 1; idx < button_height - 1; ++idx) {
    mvwaddch(content_win_, button_start_y + idx, button_start_x, ACS_VLINE);
    mvwaddch(content_win_, button_start_y + idx,
             button_start_x + button_width - 1, ACS_VLINE);
  }
  mvwaddch(content_win_, button_start_y + button_height - 1,
           button_start_x, ACS_LLCORNER);
  mvwhline(content_win_, button_start_y + button_height - 1, button_start_x + 1,
           ACS_HLINE, button_width - 2);
  mvwaddch(content_win_, button_start_y + button_height - 1,
           button_start_x + button_width - 1, ACS_LRCORNER);
  mvwprintw(content_win_, button_start_y + 1,
            x - static_cast<int>(button_text.length()) / 2,
            "%s", button_text.c_str());
  wattroff(content_win_, COLOR_PAIR(active ? ACTIVE_PAIR : DEFAULT_PAIR));
}

void SignScreen::clear_fields() {
  username_.clear();
  login_.clear();
  password_.clear();
}
