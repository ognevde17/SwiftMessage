//
// Created by sheyme on 18/03/25.
//

#ifndef SWIFTMESSAGE_SRC_SCREEN_HANDLER_CLIENT_SCREEN_HPP_
#define SWIFTMESSAGE_SRC_SCREEN_HANDLER_CLIENT_SCREEN_HPP_

#include <boost/asio.hpp>
#include <vector>
#include "abstract_screen.hpp"

enum { max_length = 1024 };

class ClientScreen : public AbstractScreen {
 public:
  ClientScreen(std::string&& host, std::string&& port)
      : host_(std::move(host)), port_(std::move(port)) {
    create_windows();
    post_create();
    setup_colors();
    update_status("Connecting to " + host_ + ":" + port_);
    redraw_all();
  }

  void update_status(const std::string& status) {
    status_ = wrap_text(status, getmaxx(contacts_win_) - 4);
    redraw_contacts();
  }

  void add_message(const std::string& msg, bool is_reply = false) {
    messages_.emplace_back(wrap_text(msg,
                                     getmaxx(chat_win_) - 4), is_reply);
    if (messages_.size() > max_messages_) {
      messages_.erase(messages_.begin());
    }
    scroll_position_ = std::max(0, static_cast<int>(
        messages_.size()) - getmaxy(chat_win_) + 3);
    redraw_chat();
  }

  int get_input_key() {
    nodelay(input_win_, true);
    int ch = wgetch(input_win_);
    nodelay(input_win_, false);
    return ch;
  }

  std::string get_input(char ch) {
    echo();
    curs_set(1);
    werase(input_win_);
    std::string beginning = "> ";
    beginning.push_back(ch);
    mvwprintw(input_win_, 1, 2, "%s", beginning.c_str());
    wmove(input_win_, 1, 5);
    wrefresh(input_win_);
    char input[max_length];
    input[0] = ch;
    wgetnstr(input_win_, input + 1, sizeof(input) - 1);
    noecho();
    curs_set(0);
    return std::string(input);
  }

  void handle_scroll(int key) {
    if (key == KEY_UP && scroll_position_ > 0) {
      scroll_position_--;
      redraw_chat();
    }
    else if (key == KEY_DOWN && scroll_position_ < static_cast<int>(
        messages_.size()) - getmaxy(chat_win_) + 3) {
      scroll_position_++;
      redraw_chat();
    }
  }

  void entering_message(char ch) { current_input_ = get_input(ch); }

  char* get_request() {
    strncpy(last_request_, current_input_.c_str(), max_length - 1);
    last_request_[max_length - 1] = '\0';
    return last_request_;
  }

  template<typename Socket>
  void catch_reply(Socket& socket) {
    char reply[max_length];
    size_t reply_length =
        socket.read_some(boost::asio::buffer(reply, max_length));
    reply[reply_length] = '\0';
    add_message("Server: " + std::string(reply), true);
  }

 protected:
  void create_windows() override {
    main_win_ = newwin(LINES, COLS, 0, 0);
    // высота LINES, ширина 25%, далее левый угол
    contacts_win_ = derwin(main_win_, LINES, COLS / 4, 0, 0);
    // высота LINES - 3, ширина 75%, далее левый угол
    chat_win_ = derwin(main_win_, LINES - 3, COLS * 3 / 4, 0, COLS / 4);
    // высота 3, ширина 75%, далее левый угол
    input_win_ = derwin(main_win_, 3, COLS * 3 / 4, LINES - 3, COLS / 4);
    box(input_win_, 1, 1);
    scrollok(chat_win_, true);
    keypad(input_win_, true);
  }

  void post_create() override {
    box(main_win_, 0, 0);
    refresh();
  }

 private:
  void setup_colors() {
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_GREEN, COLOR_WHITE);
    init_pair(4, COLOR_BLACK, COLOR_CYAN);
    wbkgd(contacts_win_, COLOR_PAIR(1));
    wbkgd(chat_win_, COLOR_PAIR(2));
    wbkgd(input_win_, COLOR_PAIR(4));
  }

  static std::string wrap_text(const std::string& text, int width) {
    std::string result;
    size_t pos = 0;
    while (pos < text.length()) {
      size_t end = std::min(pos + width, text.length());
      result += text.substr(pos, end - pos);
      if (end < text.length()) {
        result += "\n";
      }
      pos = end;
    }
    return result;
  }

  void redraw_all() {
    redraw_contacts();
    redraw_chat();
  }

  void redraw_contacts() {
    werase(contacts_win_);
    box(contacts_win_, 0, 0);
    std::string title = host_ + ":" + port_;
    mvwprintw(contacts_win_, 0,
              (getmaxx(contacts_win_) - title.length()) / 2,
              "%s", title.c_str());
    int line = 1;
    size_t start = 0;
    while (start < status_.length() && line < getmaxy(contacts_win_) - 1) {
      size_t end = status_.find('\n', start);
      if (end == std::string::npos) end = status_.length();
      mvwprintw(contacts_win_, line++, 2, "%.*s",
                static_cast<int>(end - start), status_.c_str() + start);
      start = end + 1;
    }
    wrefresh(contacts_win_);
  }

  void redraw_chat() {
    werase(chat_win_);
    box(chat_win_, 0, 0);
    int col = 1;
    int start_msg = std::max(0, scroll_position_);
    int end_msg = std::min(start_msg + getmaxy(chat_win_) - 2,
                           static_cast<int>(messages_.size()));
    for (int idx = start_msg; idx < end_msg && col < getmaxy(chat_win_) - 1; ++idx) {
      const auto& [msg, is_reply] = messages_[idx];
      wattron(chat_win_, COLOR_PAIR(is_reply ? 3 : 2));
      size_t pos = 0;
      while (pos < msg.length() && col < getmaxy(chat_win_) - 1) {
        size_t end = msg.find('\n', pos);
        if (end == std::string::npos) {
          end = msg.length();
        }
        mvwprintw(chat_win_, col++, 2, "%.*s",
                  static_cast<int>(end-pos), msg.c_str()+pos);
        pos = end + 1;
      }
      wattroff(chat_win_, COLOR_PAIR(is_reply ? 3 : 2));
    }
    if (scroll_position_ > 0 || end_msg < static_cast<int>(messages_.size())) {
      std::string scroll_ind = "^ " + std::to_string(scroll_position_+1)
          + "/" + std::to_string(messages_.size()) + " v";
      mvwprintw(chat_win_, 0, getmaxx(chat_win_) - scroll_ind.length() - 1,
                "%s", scroll_ind.c_str());
    }
    wrefresh(chat_win_);
  }

  WINDOW* contacts_win_;
  WINDOW* input_win_;
  WINDOW* chat_win_;
  std::string host_;
  std::string port_;
  std::string status_;
  std::string current_input_;
  std::vector<std::pair<std::string, bool>> messages_;
  char last_request_[max_length]{};
  const size_t max_messages_ = 1000;
  int scroll_position_ = 0;
};

#endif //SWIFTMESSAGE_SRC_SCREEN_HANDLER_CLIENT_SCREEN_HPP_
