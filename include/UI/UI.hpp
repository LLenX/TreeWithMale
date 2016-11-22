#ifndef UI_HPP
#define UI_HPP

#include <memory>
#include "Menu.hpp"

class UI {
 public:
  static std::shared_ptr<UI> getInstance() const;
  void loop();
 private:
  std::shared_ptr<Menu> topMenuPtr = new Menu();
  std::shared_ptr<Menu> curMenuPtr = nullptr;
  UI();
  ~UI();
};
#endif  // UI_HPP

