#ifndef UI_MENU_HPP
#define UI_MENU_HPP

#include "MenuItem.hpp"
#include <map>

class Menu {
 private:
 public:
  Menu();
  void show() const;
  void register();
 private:
  std::map<const std::string, std::shared_ptr<MenuItem>> items;
};

#endif  // UI_MENU_HPP
