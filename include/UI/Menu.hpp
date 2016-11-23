#ifndef UI_MENU_HPP
#define UI_MENU_HPP

#include "MenuItem.hpp"
#include <map>
#include <unordered_map>

class Menu {
 private:
 public:
  Menu();
  void show() const;
  bool register(const std::string &newOp,
                const std::shared_ptr<MenuItem> &menuItemPtr);
  bool hasOp(const std::string &op) const;
  void execOp(const std::string &op) const;
 private:
  std::unordered_map<const std::string, const std::shared_ptr<MenuItem>> items;
  std::map<const std::string, const std::string> descriptions;
  std::string description = "Menu";
  std::size_t maxOpWidth = 0;
};

#endif  // UI_MENU_HPP
