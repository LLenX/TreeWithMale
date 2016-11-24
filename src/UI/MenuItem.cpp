#include <memory>
#include "./UI/Menu.hpp"
#include "./UI/MenuItem.hpp"

using MenuItemType::Command;
using MenuItemType::SubMenu;

MenuItem::MenuItem(MenuItemType type,
                   const std::string &key, const std::string &description,
                   std::shared_ptr<Menu> i_subMenuPtr,
                   bool isShortcut, bool isCtrl)
: type(type),
  key(key), description(description),
  isShortcut(isShortcut), isCtrl(isCtrl) {
  item.subMenuPtr = i_subMenuPtr;
}

MenuItem::MenuItem(MenuItemType type,
                   const std::string &key, const std::string &description,
                   MenuItem::CmdFunc i_cmdFunc,
                   bool isShortcut, bool isCtrl)
: type(type),
  key(key), description(description),
  isShortcut(isShortcut), isCtrl(isCtrl) {
  item.commandFunc = i_cmdFunc;
}

void MenuItem::exec() const {
  switch (type) {
    case SubMenu:
      item.subMenuPtr->show();
      break;
    case Command:
      item.commandFunc();
      break;
    default:
      break;
  }
}

bool MenuItem::isSubMenu() const {
  return type == SubMenu;
}

MenuItem::ItemPtr::ItemPtr() {}
MenuItem::ItemPtr::~ItemPtr() {}
