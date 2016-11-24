#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
#include <algorithm>
#include "./UI/Menu.hpp"
#include "./UI/MenuItem.hpp"

Menu::Menu(const std::string &i_description)
: description(i_description) {

}
void coutFence(const std::string &description, bool top = true) {
  std::cout << "--------------------" << (top ? std::string(" ") + description + " " : std::string(description.length() + 2, '-')) << "-----------------------" << std::endl;
};
void Menu::show() const {
  coutFence(description);
  std::cout << std::endl;
  if (items.empty()) {
    std::cout << "No items available" << std::endl;
  } else {
    for (auto &oneItem : items) {
      if (oneItem.second->isShortcut) continue;
      std::cout << std::setw(static_cast<int>(maxOpWidth))
                << oneItem.first << " - " << oneItem.second->description
                << std::endl;
    }
  }
  std::cout << std::endl;
  coutFence(description, false);
}
bool Menu::addOp(const std::shared_ptr<MenuItem> &menuItemPtr) {
  if (hasOp(menuItemPtr->key)) return false;
  items[menuItemPtr->key] = menuItemPtr;
  maxOpWidth = std::max(maxOpWidth, menuItemPtr->key.length());
  return true;
}
bool Menu::hasOp(const std::string &op) const {
  return items.find(op) != items.end();
}
void Menu::execOp(const std::string &op) const {
  if (not hasOp(op)) {
    return;
  }
  items.at(op)->exec();
}
