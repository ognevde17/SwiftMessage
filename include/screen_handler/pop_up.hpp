//
// Created by sheyme on 17/05/25.
//

#ifndef SWIFTMESSAGE_INCLUDE_SCREEN_HANDLER_POP_UP_HPP_
#define SWIFTMESSAGE_INCLUDE_SCREEN_HANDLER_POP_UP_HPP_

#include <vector>

#include "abstract_screen.hpp"

class ColorPopUp : public AbstractScreen {
 public:
  ColorPopUp();

  void show();
  void hide();
  bool is_visible() const { return is_visible_; }

  void refresh() override;

  Result handle_input(char ch);
  int get_selected_color() const { return data_[selected_index_].second; }

  void handle_resize();

  ~ColorPopUp() override;

 protected:
  void create_windows() override;

 private:
  void draw_shadow();

  void draw_selector();

  WINDOW* popup_win_{nullptr};
  bool is_visible_{false};
  size_t selected_index_{0};
  std::vector<std::pair<std::string, int>> data_;
};

#endif  // SWIFTMESSAGE_INCLUDE_SCREEN_HANDLER_POP_UP_HPP_
