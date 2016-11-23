#ifndef UI_HPP
#define UI_HPP

#include <string>
#include <memory>
#include <functional>
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
  std::string &&UI::inputOp(std::function<bool (*)(const std::string &)> isValid);
  static const char *prompt = ">>> ";
};
#endif  // UI_HPP

