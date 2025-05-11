//
// Created by sheyme on 24/04/25.
//

#include "../../include/screen_handler/chat_screen.hpp"

ChatScreen::ChatScreen() : AbstractScreen() {
  create_windows();
  post_create();
  refresh();
}

void ChatScreen::create_windows() {
  AbstractScreen::create_windows();
  int contacts_width = COLS / 4;
  contacts_win_ = derwin(main_win_, LINES, contacts_width, 0, 0);
  chat_win_ = derwin(main_win_, LINES - 3, COLS - contacts_width,
                     0, contacts_width);
  input_win_ = derwin(main_win_, 3, COLS - contacts_width,
                      LINES - 3, contacts_width);
  keypad(input_win_, true);
  scrollok(chat_win_, true);
  wtimeout(input_win_, 100);
}

void ChatScreen::post_create() {
  box(main_win_, 0, 0);
  wrefresh(main_win_);
  draw_background();
}

void ChatScreen::refresh() {
  werase(main_win_);
  draw_layout();
}

void ChatScreen::update_username(const std::string& username) {
  username_ = username;
}

void ChatScreen::load_messages(std::vector<Message>&& messages) {
  messages_ = std::move(messages);
}

void ChatScreen::load_messages(const std::vector<Message>& messages) {
  messages_ = messages;
}

void ChatScreen::add_messages_update(const std::vector<Message>& messages) {
  messages_.insert(messages_.end(), messages.begin(), messages.end());
}

std::string ChatScreen::get_current_input() {
  auto input_copy = current_input_;
  current_input_.clear();
  return input_copy;
}

ChatScreen::Result ChatScreen::handle_input() {
  int ch = wgetch(input_win_);
  switch (ch) {
    case KEY_RESIZE:
      handle_resize();
      return Result::None;
    case KEY_UP:
      handle_scroll(-1);
      return Result::Scroll;
    case KEY_DOWN:
      handle_scroll(1);
      return Result::Scroll;
    case 9:
      handle_scroll(1);
      return Result::Scroll;
    case 27:
      return Result::Exit;
    case '\n':
      refresh();
      return Result::NewMessage;
    case KEY_ENTER:
      refresh();
      return Result::NewMessage;
    default:
      handle_char(ch);
      return Result::None;
  }
}

std::string ChatScreen::wrap_text(const std::string& text, int width) {
  std::string result;
  size_t line_start = 0;
  size_t last_space = 0;
  for (size_t idx = 0; idx < text.size(); ++idx) {
    if (text[idx] == ' ') {
      last_space = idx;
    }
    if (idx - line_start >= static_cast<size_t>(width)) {
      if (last_space > line_start) {
        result += text.substr(line_start, last_space - line_start) + '\n';
        line_start = last_space + 1;
        last_space = line_start;
      } else {
        result += text.substr(line_start, width) + '\n';
        line_start += width;
      }
    }
  }
  if (line_start < text.size()) {
    result += text.substr(line_start);
  }
  return result;
}

void ChatScreen::add_message(const std::string& message, ColorPairs type) {
  messages_.push_back({wrap_text(message, getmaxx(chat_win_) - 4), type});
  if (messages_.size() > static_cast<size_t>(kMaxMessages)) {
    messages_.erase(messages_.begin());
  }
  scroll_position_ =
      std::max(0, static_cast<int>(messages_.size()) - getmaxy(chat_win_) + 2);
  draw_chat();
}

void ChatScreen::clear_chat() {
  messages_.clear();
  scroll_position_ = 0;
  draw_chat();
}

void ChatScreen::handle_char(int ch) {
  if (ch == KEY_BACKSPACE || ch == 127) {
    if (!current_input_.empty()) {
      current_input_.pop_back();
    }
  } else if (isprint(ch) != 0) {
    current_input_ += static_cast<char>(ch);
  }
  draw_input_field();
}

void ChatScreen::handle_resize() {
  delwin(contacts_win_);
  delwin(input_win_);
  delwin(chat_win_);
  create_windows();
  draw_background();
  refresh();
}

void ChatScreen::handle_scroll(int direction) {
  int visible_line = getmaxy(chat_win_) - 2;
  int total_messages = static_cast<int>(messages_.size());
  int max_scroll = std::max(0, total_messages - visible_line);
  int new_position = scroll_position_ + direction;
  if (new_position < 0) {
    new_position = 0;
  } else if (new_position > max_scroll) {
    new_position = max_scroll;
  }
  scroll_position_ = new_position;
  draw_chat();
}

void ChatScreen::draw_background() {
  if (has_colors()) {
    wbkgd(contacts_win_, COLOR_PAIR(ACTIVE_PAIR));
    wbkgd(chat_win_, COLOR_PAIR(ACTIVE_PAIR));
    wbkgd(input_win_, COLOR_PAIR(ACTIVE_PAIR));
  }
}

void ChatScreen::draw_contacts() {
  werase(contacts_win_);
  box(contacts_win_, 0, 0);
  apply_color(contacts_win_, DEFAULT_PAIR, true);
  mvwprintw(contacts_win_, 0, (getmaxx(contacts_win_) - username_.length()) / 2,
            "%s", username_.c_str());
  apply_color(contacts_win_, DEFAULT_PAIR, false);
  int line = 2;
  size_t start = 0;
  while (start < status_.length() && line < static_cast<size_t>
                                         (getmaxy(contacts_win_) - 1)) {
    size_t end = status_.find('\n', start);
    if (end == std::string::npos) {
      end = status_.length();
    }
    mvwprintw(contacts_win_, line++, 2, "%.*s",
              static_cast<int>(end - start), status_.c_str() + start);
    start = end + 1;
  }
  wrefresh(contacts_win_);
}

void ChatScreen::draw_chat() {
  werase(chat_win_);
  box(chat_win_, 0, 0);
  int col = 1;
  int start_msg = std::max(0, scroll_position_);
  int end_msg = std::min(start_msg + getmaxy(chat_win_) - 2,
                         static_cast<int>(messages_.size()));
  for (int idx = start_msg; idx < end_msg && col < getmaxy(chat_win_) - 1;
       ++idx) {
    const auto& message = messages_[idx];
    apply_color(chat_win_, message.type, true);
    size_t pos = 0;
    while (pos < message.text.length() && col < getmaxy(chat_win_) - 1) {
      size_t end = message.text.find('\n', pos);
      if (end == std::string::npos) {
        end = message.text.length();
      }
      mvwprintw(chat_win_, col++, 2, "%.*s",
                static_cast<int>(end-pos), message.text.c_str()+pos);
      pos = end + 1;
    }
    apply_color(chat_win_, message.type, false);
  }
  if (scroll_position_ > 0 || end_msg < static_cast<int>(messages_.size())) {
    apply_color(chat_win_, DEFAULT_PAIR, true);
    std::string scroll_ind = "^ " + std::to_string(scroll_position_+1)
                             + "/" + std::to_string(messages_.size()) + " v";
    mvwprintw(chat_win_, 0, getmaxx(chat_win_) - scroll_ind.length() - 1,
              "%s", scroll_ind.c_str());
    apply_color(chat_win_, DEFAULT_PAIR, false);
  }
  wrefresh(chat_win_);
}

void ChatScreen::draw_input_field() {
  werase(input_win_);
  box(input_win_, 0, 0);
  mvwprintw(input_win_, 1, 2, "> ");
  apply_color(input_win_, DEFAULT_PAIR, true);
  mvwprintw(input_win_, 1, 4, "%s", current_input_.c_str());
  apply_color(input_win_, DEFAULT_PAIR, false);
  wmove(input_win_, 1, 2 + current_input_.length() + 1);
  wrefresh(input_win_);
}

void ChatScreen::draw_layout() {
  draw_contacts();
  draw_chat();
  draw_input_field();
  wrefresh(main_win_);
}

ChatScreen::~ChatScreen() {
  delwin(contacts_win_);
  delwin(chat_win_);
  delwin(input_win_);
}
