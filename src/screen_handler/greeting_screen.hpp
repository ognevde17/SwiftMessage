//
// Created by sheyme on 03/04/25.
//

#ifndef SWIFTMESSAGE_SRC_SCREEN_HANDLER_GREETING_SCREEN_HPP_
#define SWIFTMESSAGE_SRC_SCREEN_HANDLER_GREETING_SCREEN_HPP_

#include "abstract_screen.hpp"

class GreetingScreen : public AbstractScreen {
 public:
  GreetingScreen() : AbstractScreen() {
    curs_set(0);
    setup_colors();
    draw_ui();
  }

  bool handle_input() {
    nodelay(main_win_, true);
    int ch = wgetch(main_win_);
    if (ch == KEY_RESIZE) {
      handle_resize();
      return false;
    }
    return ch == '\n' || ch == KEY_ENTER || ch == ' ';
  }

  void refresh() override {
    AbstractScreen::refresh();
    draw_ui();
  }

 protected:
  void create_windows() override {
    main_win_ = newwin(LINES, COLS, 0, 0);
    keypad(main_win_, true);
    wtimeout(main_win_, 100);
 }

 private:
  void setup_colors() {
    if (has_colors()) {
      init_pair(BUTTON_PAIR, COLOR_BLACK, COLOR_WHITE);
      init_pair(GREETING_SCREEN_PAIR, COLOR_WHITE, COLOR_BLUE);
    }
    wbkgd(main_win_, COLOR_PAIR(GREETING_SCREEN_PAIR));
  }

  void handle_resize() {
    wclear(main_win_);
    delwin(main_win_);
    create_windows();
    setup_colors();
    draw_ui();
  }

  void draw_ui() {
    werase(main_win_);
    box(main_win_, 0, 0);
    int center_y = LINES / 2;
    int center_x = COLS / 2;

    std::string greeting = "Hello!";
    wattron(main_win_, COLOR_PAIR(GREETING_SCREEN_PAIR));
    mvwprintw(main_win_, center_y - 4, center_x - greeting.length()/2, "%s", greeting.c_str());
    wattroff(main_win_, COLOR_PAIR(GREETING_SCREEN_PAIR));

    std::string button_text = " Continue ";
    int button_width = button_text.length() + 2;
    int button_height = 3;
    int button_start_x = center_x - button_width/2;
    int button_start_y = center_y + 1;
    wattron(main_win_, COLOR_PAIR(BUTTON_PAIR));
    mvwaddch(main_win_, button_start_y, button_start_x, ACS_ULCORNER);
    mvwhline(main_win_, button_start_y, button_start_x + 1, ACS_HLINE, button_width - 2);
    mvwaddch(main_win_, button_start_y, button_start_x + button_width - 1, ACS_URCORNER);
    for(int i = 1; i < button_height - 1; i++) {
      mvwaddch(main_win_, button_start_y + i, button_start_x, ACS_VLINE);
      mvwaddch(main_win_, button_start_y + i, button_start_x + button_width - 1, ACS_VLINE);
    }
    mvwaddch(main_win_, button_start_y + button_height - 1, button_start_x, ACS_LLCORNER);
    mvwhline(main_win_, button_start_y + button_height - 1, button_start_x + 1, ACS_HLINE, button_width - 2);
    mvwaddch(main_win_, button_start_y + button_height - 1, button_start_x + button_width - 1, ACS_LRCORNER);
    mvwprintw(main_win_, button_start_y + 1, center_x - button_text.length()/2, "%s", button_text.c_str());
    wattroff(main_win_, COLOR_PAIR(BUTTON_PAIR));

    wrefresh(main_win_);
  }
};

#endif //SWIFTMESSAGE_SRC_SCREEN_HANDLER_GREETING_SCREEN_HPP_
