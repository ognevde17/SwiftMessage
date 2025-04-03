//
// Created by sheyme on 04/04/25.
//

#ifndef SWIFTMESSAGE_SRC_SCREEN_HANDLER_CHAT_SCREEN_HPP_
#define SWIFTMESSAGE_SRC_SCREEN_HANDLER_CHAT_SCREEN_HPP_

#include "abstract_screen.hpp"
#include <vector>

enum { max_length = 1024 };

class ChatScreen : public AbstractScreen {
 public:
  ChatScreen() : AbstractScreen() {
    create_windows();
    post_create();
    setup_colors();
    redraw_chat();
  }

  void RenderChat(std::vector<std::pair<std::string, bool>>& chat) {
    chat_.clear();
    chat_ = chat;
    redraw_chat();
  }

  void add_message(const std::string& msg, bool is_reply = false) {
    chat_.emplace_back(wrap_text(msg, getmaxx(chat_win_) - 4), is_reply);
    if(chat_.size() > max_messages_) {
      chat_.erase(chat_.begin());
    }
    scroll_position_ = std::max(0, static_cast<int>(chat_.size()) - getmaxy(chat_win_) + 3);
    redraw_chat();
  }

  bool process_input() {
    int ch = get_input_key();
    switch (ch) {
      case ERR: {
        break;
      }
      case KEY_RESIZE: {
        handle_resize();
        break;
      }
      case KEY_ENTER: {
        break;
      }
      case KEY_UP: {
        handle_scroll(ch);
        break;
      }
      case KEY_DOWN: {
        handle_scroll(ch);
        break;
      }
      default: {
        handle_input(ch);
        return true;
      }
    }
    return false;
  }

  ~ChatScreen() override {
    delwin(input_win_);
    delwin(chat_win_);
  }

 protected:
  void create_windows() override {
    main_win_ = newwin(LINES, COLS, 0, 0);
    chat_win_ = derwin(main_win_, LINES - 3, COLS, 0, 0);
    input_win_ = derwin(main_win_, 3, COLS, LINES - 3, 0);
    box(input_win_, 0, 0);
    scrollok(chat_win_, true);
    keypad(input_win_, true);
  }

  void post_create() override {
    box(main_win_, 0, 0);
    refresh();
  }

 private:
  void setup_colors() {
    if(has_colors()) {
      init_pair(CHAT_PAIR, COLOR_BLACK, COLOR_WHITE);
      init_pair(REPLY_PAIR, COLOR_GREEN, COLOR_WHITE);
      init_pair(INPUT_PAIR, COLOR_BLACK, COLOR_CYAN);
    }
    wbkgd(chat_win_, COLOR_PAIR(CHAT_PAIR));
    wbkgd(input_win_, COLOR_PAIR(INPUT_PAIR));
  }

  static std::string wrap_text(const std::string& text, int width) {
    std::string result;
    size_t line_start = 0;
    size_t last_space = 0;
    for(size_t i = 0; i < text.size(); ++i) {
      if(text[i] == ' ') {
        last_space = i;
      }
      if(i - line_start >= static_cast<size_t>(width)) {
        if(last_space > line_start) {
          result += text.substr(line_start, last_space - line_start) + '\n';
          line_start = last_space + 1;
        } else {
          result += text.substr(line_start, width) + '\n';
          line_start += width;
        }
        last_space = line_start;
      }
    }
    result += text.substr(line_start);
    return result;
  }

  int get_input_key() {
    nodelay(input_win_, true);
    int ch = wgetch(input_win_);
    nodelay(input_win_, false);
    return ch;
  }

  void handle_input(int ch) {
    echo();
    curs_set(1);
    werase(input_win_);
    std::string input;
    mvwprintw(input_win_, 1, 2, "> ");
    wmove(input_win_, 1, 5);
    char buf[max_length]{};
    buf[0] = ch;
    wgetnstr(input_win_, buf + 1, sizeof(buf)-1);
    input += buf;
    noecho();
    curs_set(0);
    current_input_ = input;
  }

  void handle_scroll(int key) {
    if(key == KEY_UP && scroll_position_ > 0) {
      scroll_position_--;
    } else if(key == KEY_DOWN && scroll_position_ < static_cast<int>(
        chat_.size()) - getmaxy(chat_win_) + 3) {
      scroll_position_++;
    }
    redraw_chat();
  }

  void handle_resize() {
    delwin(input_win_);
    delwin(chat_win_);
    create_windows();
    setup_colors();
    redraw_chat();
  }

  void redraw_chat() {
    werase(chat_win_);
    box(chat_win_, 0, 0);

    const int max_lines = getmaxy(chat_win_) - 2;
    const int start = std::max(0, static_cast<int>(chat_.size()) - max_lines);

    int line = 1;
    for(size_t i = start; i < chat_.size() && line <= max_lines; ++i) {
      const auto& [msg, is_reply] = chat_[i];
      wattron(chat_win_, COLOR_PAIR(is_reply ? REPLY_PAIR : CHAT_PAIR));

      size_t pos = 0;
      while(pos < msg.length() && line <= max_lines) {
        size_t end = msg.find('\n', pos);
        if(end == std::string::npos) end = msg.length();
        mvwprintw(chat_win_, line++, 2, "%.*s",
                  static_cast<int>(end - pos), msg.c_str() + pos);
        pos = end + 1;
      }
      wattroff(chat_win_, COLOR_PAIR(is_reply ? REPLY_PAIR : CHAT_PAIR));
    }

    if(scroll_position_ > 0 || start + max_lines < static_cast<int>(chat_.size())) {
      std::string scroll_ind = "^ " + std::to_string(scroll_position_+1) + "/"
          + std::to_string(chat_.size()) + " v";
      mvwprintw(chat_win_, 0, getmaxx(chat_win_) - scroll_ind.length() - 1,
                "%s", scroll_ind.c_str());
    }
    wrefresh(chat_win_);
  }

  WINDOW* input_win_;
  WINDOW* chat_win_;
  std::string current_input_;
  std::vector<std::pair<std::string, bool>> chat_;
  const size_t max_messages_{1000};
  int scroll_position_{0};
};


#endif //SWIFTMESSAGE_SRC_SCREEN_HANDLER_CHAT_SCREEN_HPP_
