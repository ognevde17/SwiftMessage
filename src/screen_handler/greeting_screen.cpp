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
  wbkgd(main_win_, COLOR_PAIR(ACTIVE_PAIR));
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
  int center_y = LINES / 2;
  int center_x = COLS / 2;

  std::string greeting = "Hello!";
  wattron(main_win_, COLOR_PAIR(ACTIVE_PAIR));
  mvwprintw(main_win_, center_y - 4,
            center_x - static_cast<int>(greeting.length()) / 2,
            "%s", greeting.c_str());
  wattroff(main_win_, COLOR_PAIR(ACTIVE_PAIR));

  std::string button_text = " Continue ";
  int button_width = static_cast<int>(button_text.length()) + 2;
  int button_height = 3;
  int button_start_x = center_x - button_width/2;
  int button_start_y = center_y + 1;
  wattron(main_win_, COLOR_PAIR(ACTIVE_PAIR));
  mvwaddch(main_win_, button_start_y, button_start_x, ACS_ULCORNER);
  mvwhline(main_win_, button_start_y, button_start_x + 1,
           ACS_HLINE, button_width - 2);
  mvwaddch(main_win_, button_start_y, button_start_x + button_width - 1,
           ACS_URCORNER);
  for(int i = 1; i < button_height - 1; i++) {
    mvwaddch(main_win_, button_start_y + i, button_start_x, ACS_VLINE);
    mvwaddch(main_win_, button_start_y + i,
             button_start_x + button_width - 1, ACS_VLINE);
  }
  mvwaddch(main_win_, button_start_y + button_height - 1,
           button_start_x, ACS_LLCORNER);
  mvwhline(main_win_, button_start_y + button_height - 1, button_start_x + 1,
           ACS_HLINE, button_width - 2);
  mvwaddch(main_win_, button_start_y + button_height - 1,
           button_start_x + button_width - 1, ACS_LRCORNER);
  mvwprintw(main_win_, button_start_y + 1,
            center_x - static_cast<int>(button_text.length()) / 2,
            "%s", button_text.c_str());
  wattroff(main_win_, COLOR_PAIR(ACTIVE_PAIR));

  wrefresh(main_win_);
}