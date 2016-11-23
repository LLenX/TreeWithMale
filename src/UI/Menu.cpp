#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
#include <algorithm>
#include "Menu.hpp"
Menu::Menu() {

}
void coutFence(const std::string &description, bool top = true) {
  std::cout << "--------------------" << (top ? std::string(" ") + description + " " : std::string(description.length() + 2, '-')) << "-----------------------" << std::endl;
};
void Menu::show() const {
  coutFence(description);
  for (auto &oneDescription : descriptions) {
    std::cout << std::setw(maxOpWidth) << oneDescription.first << " - " << oneDescription.second << std::endl;
  }
  coutFence(description, false);
}
bool Menu::register(const std::string &newOp,
                    const std::shared_ptr<MenuItem> &menuItemPtr) {
  bool success = items.insert({newOp, menuItemPtr}).second;
  if (!success) return false;
  maxOpWidth = std::max(maxOpWidth, newOp.length());
}
bool Menu::hasOp(const std::string &op) const {
  return item.find(op) != items.end();
}
void execOp(const std::string &op) const {
  if (!hasOp(op)) {
    return;
  }
  (*items[op])();
}
