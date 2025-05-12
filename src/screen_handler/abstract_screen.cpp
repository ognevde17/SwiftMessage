//
// Created by sheyme on 24/04/25.
//

#include "../../include/screen_handler/abstract_screen.hpp"

AbstractScreen::AbstractScreen() {
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

Format AbstractScreen::get_format(const ColorPairs& pair) {
  static const std::unordered_map<ColorPairs, Format> format_map = {
      {DEFAULT_PAIR, {DEFAULT_PAIR, TextAttribute::NORMAL}},
      {ACTIVE_PAIR, {ACTIVE_PAIR, TextAttribute::BOLD}},
      {SYSTEM_NOTIFICATION_PAIR,
       {SYSTEM_NOTIFICATION_PAIR, TextAttribute::BLINK}}
  };
  return format_map.at(pair);
}

void AbstractScreen::apply_color(WINDOW* win, const ColorPairs& pair,
                                 bool state) {
  const auto& [color, attribute] = get_format(pair);
  if (state) {
    if (has_colors()) {
      wattron(win, COLOR_PAIR(color));
    } else {
      wattron(win, attribute);
    }
  } else {
    if (has_colors()) {
      wattroff(win, COLOR_PAIR(color));
    } else {
      wattroff(win, attribute);
    }
  }
}

void AbstractScreen::draw_button(WINDOW* win, int x, int y, ColorPairs color,
                                 const std::string& label) {
  int button_width = static_cast<int>(label.length()) + 2;
  int button_height = 3;
  int button_start_x = x - button_width / 2;
  int button_start_y = y + 1;
  apply_color(win, color, true);
  mvwaddch(win, button_start_y, button_start_x, ACS_ULCORNER);
  mvwhline(win, button_start_y, button_start_x + 1,
           ACS_HLINE, button_width - 2);
  mvwaddch(win, button_start_y, button_start_x + button_width - 1,
           ACS_URCORNER);
  for (int idx = 1; idx < button_height - 1; ++idx) {
    mvwaddch(win, button_start_y + idx, button_start_x, ACS_VLINE);
    mvwaddch(win, button_start_y + idx,
             button_start_x + button_width - 1, ACS_VLINE);
  }
  mvwaddch(win, button_start_y + button_height - 1,
           button_start_x, ACS_LLCORNER);
  mvwhline(win, button_start_y + button_height - 1, button_start_x + 1,
           ACS_HLINE, button_width - 2);
  mvwaddch(win, button_start_y + button_height - 1,
           button_start_x + button_width - 1, ACS_LRCORNER);
  mvwprintw(win, button_start_y + 1,
            x - static_cast<int>(label.length()) / 2,
            "%s", label.c_str());
  apply_color(win, color, false);
}

AbstractScreen::~AbstractScreen() {
  delwin(main_win_);
  delwin(content_win_);
  endwin();
}