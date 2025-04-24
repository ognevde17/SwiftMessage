//
// Created by sheyme on 23/04/25.
//

#ifndef SWIFTMESSAGE_SRC_SCREEN_HANDLER_SIGN_SCREEN_HPP_
#define SWIFTMESSAGE_SRC_SCREEN_HANDLER_SIGN_SCREEN_HPP_

#include "abstract_screen.hpp"

class SignScreen : public AbstractScreen {
 public:
  enum class Result {
    None,
    Login,
    Register,
    Exit
  };

  SignScreen() : AbstractScreen() {
    init_colors();
    refresh();
  }

  std::string get_username() const { return username_; }
  std::string get_login() const { return login_; }
  std::string get_password() const { return password_; }

  void refresh() override {
    werase(content_win_);
    werase(main_win_);
    draw_borders();
    draw_fields();
    draw_switcher();
    draw_submit_button();
    AbstractScreen::refresh();
  }

  Result handle_input() {
    int ch = wgetch(content_win_);
    switch (ch) {
      case (KEY_RESIZE): {
        handle_resize();
        return Result::None;
      }
      case (KEY_UP): {
        move_cursor(-1);
        return Result::None;
      }
      case (KEY_DOWN): {
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

 protected:
  void create_windows() override {
    AbstractScreen::create_windows();
    keypad(content_win_, true);
  }

 private:
  void init_colors() {
    if (has_colors()) {
      init_pair(ACTIVE_PAIR, COLOR_CYAN, COLOR_BLACK);
      init_pair(DEFAULT_PAIR, COLOR_WHITE, COLOR_BLACK);
    }
    wbkgd(main_win_, COLOR_PAIR(DEFAULT_PAIR));
    wbkgd(content_win_, COLOR_PAIR(DEFAULT_PAIR));
  }

  void handle_resize() {
    delwin(main_win_);
    delwin(content_win_);
    create_windows();
    wbkgd(main_win_, COLOR_PAIR(DEFAULT_PAIR));
    wbkgd(content_win_, COLOR_PAIR(DEFAULT_PAIR));
    refresh();
  }

  void handle_char(int ch) {
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

  Result handle_submit() {
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

  void move_cursor(int direction) {
    int max_fields = is_registration_ ? 5 : 4;
    current_field_ = (current_field_ + direction + max_fields) % max_fields;
    refresh();
  }

  std::string* get_current_field() {
    if (is_registration_) {
      switch (current_field_) {
        case (0): return &username_;
        case (1): return &login_;
        case (2): return &password_;
      }
    } else {
      switch(current_field_) {
        case (0): return &login_;
        case (1): return &password_;
      }
    }
    return nullptr;
  }

  void draw_borders() {
    box(main_win_, 0, 0);
  }

  void draw_field(const std::string& label, const std::string& value,
                  int x, int y, int field_num) {
    bool active = (field_num == current_field_);
    wattron(content_win_, COLOR_PAIR(active ? ACTIVE_PAIR : DEFAULT_PAIR));
    mvwprintw(content_win_, y, x, "%s", label.c_str());
    wprintw(content_win_, "[%-20s]", value.c_str());
    wattroff(content_win_, COLOR_PAIR(active ? ACTIVE_PAIR : DEFAULT_PAIR));
  }

  void draw_fields() {
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

  void draw_switcher() {
    int x = COLS / 2 - 4;
    int y = LINES / 2 - (is_registration_ ? 0 : 1);
    wattron(content_win_, COLOR_PAIR(current_field_ == (is_registration_ ? 3 : 2) ? ACTIVE_PAIR : DEFAULT_PAIR));
    if (is_registration_) {
      mvwprintw(content_win_, y, x, "Sign In");
    } else {
      mvwprintw(content_win_, y, x, "Sign Up");
    }
    wattroff(content_win_, COLOR_PAIR(current_field_ == (is_registration_ ? 3 : 2) ? ACTIVE_PAIR : DEFAULT_PAIR));
  }

  void draw_submit_button() {
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
    for(int i = 1; i < button_height - 1; i++) {
      mvwaddch(content_win_, button_start_y + i, button_start_x, ACS_VLINE);
      mvwaddch(content_win_, button_start_y + i,
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

  void clear_fields() {
    username_.clear();
    login_.clear();
    password_.clear();
  }

  bool is_registration_{false};
  int current_field_{0};
  std::string username_;
  std::string login_;
  std::string password_;
};

#endif //SWIFTMESSAGE_SRC_SCREEN_HANDLER_SIGN_SCREEN_HPP_
