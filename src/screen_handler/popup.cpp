//
// Created by sheyme on 17/05/25.
//

#include "../../include/screen_handler/pop_up.hpp"

ColorPopUp::ColorPopUp() {
  data_ = {
    {"Cyan", COLOR_CYAN},
    {"Brown", COLOR_YELLOW},
    {"Blue", COLOR_BLUE},
    {"Green", COLOR_GREEN}
  };
  create_windows();
}

ColorPopUp::~ColorPopUp() {
  if (popup_win_ != nullptr) {
    delwin(popup_win_);
  }
}

void ColorPopUp::create_windows() {
  int height = data_.size() + 4;
  int width = 20;
  int x = (COLS - width) / 2;
  int y = (LINES - height) / 2;
  popup_win_ = newwin(height, width, y, x);
}

void ColorPopUp::show() {
  if (popup_win_ == nullptr) {
    return;
  }
  is_visible_ = true;
  refresh();
}

void ColorPopUp::hide() {
  is_visible_ = false;
  if (popup_win_ != nullptr) {
    werase(popup_win_);
    wrefresh(popup_win_);
  }
}

void ColorPopUp::refresh() {
  draw_shadow();
  draw_selector();
  wrefresh(popup_win_);
}

AbstractScreen::Result ColorPopUp::handle_input(char ch) {
  if (is_visible()) {
    switch (ch) {
      case KEY_RESIZE:
        handle_resize();
        return Result::Login;
      case '\n':
        hide();
        if (has_colors()) {
          init_pair(ACTIVE_PAIR, get_selected_color(), COLOR_BLACK);
        }
        return Result::Login;
      case KEY_ENTER:
        hide();
        if (has_colors()) {
          init_pair(ACTIVE_PAIR, get_selected_color(), COLOR_BLACK);
        }
        return Result::Login;
      case '\t':
        if (selected_index_ < 3) {
          ++selected_index_;
        } else {
          selected_index_ = 0;
        }
        refresh();
        return Result::Login;
      case 27:
        hide();
        refresh();
        return Result::Login;
    }
  }
  if (ch == '\t') {
    show();
    return Result::Login;
  }
  return Result::None;
}

void ColorPopUp::draw_shadow() {
  int height = data_.size() + 4;
  int width = 20;
  wattron(popup_win_, A_DIM);
  mvwhline(popup_win_, height - 1, 2, ACS_CKBOARD, width - 2);
  mvwvline(popup_win_, 1, width - 1, ACS_CKBOARD, height - 2);
  wattroff(popup_win_, A_DIM);
}

void ColorPopUp::draw_selector() {
  wbkgd(popup_win_, COLOR_PAIR(ACTIVE_PAIR));
  apply_color(popup_win_, ACTIVE_PAIR, true);
  box(popup_win_, 0, 0);
  std::string prompt = " Select color ";
  mvwprintw(popup_win_, 0, (getmaxx(popup_win_) / 2) - (prompt.length() / 2),
            "%s", prompt.c_str());
  apply_color(popup_win_, ACTIVE_PAIR, false);
  for (size_t idx = 0; idx < data_.size(); ++idx) {
    auto current_color = idx == selected_index_ ? ACTIVE_PAIR : DEFAULT_PAIR;
    apply_color(popup_win_, current_color, true);
    mvwprintw(popup_win_, idx + 2, 2, "%s", data_[idx].first.c_str());
    apply_color(popup_win_, current_color, false);
  }
}

void ColorPopUp::handle_resize() {
  wclear(popup_win_);
  delwin(popup_win_);
  create_windows();
  refresh();
}