#include <memory>
#include "./UI/Menu.hpp"
#include "./UI/MenuItem.hpp"

using MenuItemType::Command;
using MenuItemType::SubMenu;

MenuItem::MenuItem(MenuItemType type,
                   const std::string &key, const std::string &description,
                   std::shared_ptr<Menu> subMenuPtr,
                   bool isShortcut, bool isCtrl)
: type(type), key(key), description(description),
  subMenuPtr(subMenuPtr),
  isShortcut(isShortcut), isCtrl(isCtrl) {}

MenuItem::MenuItem(MenuItemType type,
                   const std::string &key, const std::string &description,
                   MenuItem::CmdFunc commandFunc,
                   bool isShortcut, bool isCtrl)
: type(type), key(key), description(description),
  commandFunc(commandFunc),
  isShortcut(isShortcut), isCtrl(isCtrl) {}

void MenuItem::exec() const {
  switch (type) {
    case SubMenu:
      subMenuPtr->show();
      break;
    case Command:
      commandFunc();
      break;
    default:
      break;
  }
}

bool MenuItem::isSubMenu() const {
  return type == SubMenu;
}
