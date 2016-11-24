#ifndef TREEWITHMALE_UI_MENUITEM_HPP_
#define TREEWITHMALE_UI_MENUITEM_HPP_

#include <functional>
#include <string>
#include <memory>

class Menu;
class UIController;

enum MenuItemType {
  SubMenu,
  Command
};

class MenuItem {
 private:
  typedef std::function<void()> CmdFunc;
 public:
  friend class Menu;
  MenuItem(MenuItemType type,
           const std::string &key, const std::string &description,
           std::shared_ptr<Menu> subMenuPtr,
           bool isShortcut = false, bool isCtrl = false);
  MenuItem(MenuItemType type,
           const std::string &key, const std::string &description,
           CmdFunc commandFunc,
           bool isShortcut = false, bool isCtrl = false);
  void exec() const;
  bool isSubMenu() const;
 private:
  const MenuItemType type;
  const std::string key;
  const std::string description;
  std::shared_ptr<Menu> subMenuPtr;
  CmdFunc commandFunc;
  const bool isShortcut;
  const bool isCtrl;
};

#endif  // TREEWITHMALE_UI_MENUITEM_HPP_
