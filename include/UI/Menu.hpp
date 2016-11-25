#ifndef TREEWITHMALE_UI_MENU_HPP_
#define TREEWITHMALE_UI_MENU_HPP_

#include <map>
#include <string>

class MenuItem;

class Menu {
 private:
 public:
  Menu(const std::string &i_description);
  void show() const;
  bool addOp(const std::shared_ptr<MenuItem> &menuItemPtr);
  bool hasOp(const std::string &op) const;
  void execOp(const std::string &op) const;
  bool opIsSubMenu(const std::string &op) const;
  std::shared_ptr<Menu> getSubMenuPtrOf(const std::string &op) const;
 private:
  std::map<std::string, std::shared_ptr<MenuItem>> items;
  const std::string description;
  std::size_t maxOpWidth = 0;
};

#endif  // TREEWITHMALE_UI_MENU_HPP_
