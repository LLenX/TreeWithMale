#include <memory>
#include "./UI/Menu.hpp"
#include "./UI/MenuItem.hpp"

using MenuItemType::Command;
using MenuItemType::SubMenu;

MenuItem::MenuItem(MenuItemType i_type,
                   const std::string &i_key, const std::string &i_description,
                   std::shared_ptr<Menu> i_subMenuPtr,
                   bool i_isShortcut)
: type(i_type),
  key(i_key), description(i_description),
  isShortcut(i_isShortcut) {
  item.subMenuPtr = i_subMenuPtr;
}

MenuItem::MenuItem(MenuItemType i_type,
                   const std::string &i_key, const std::string &i_description,
                   MenuItem::CmdFunc i_cmdFunc,
                   bool i_isShortcut)
: type(i_type),
  key(i_key), description(i_description),
  isShortcut(i_isShortcut) {
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
