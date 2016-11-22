#include "./UI/MenuItem.hpp"
MenuItem(ItemType i_itemType, const std::string &i_key, std::shared_ptr<Menu> i_subMenuPtr)
: itemType(i_itemType), key(i_key), item() {
  item.subMenuPtr = i_subMenuPtr;
}

MenuItem(ItemType i_itemType, const std::string &i_key, std::shared_ptr<CmdFunc> i_cmdFunc)
: itemType(i_itemType), key(i_key), item() {
  item.command = i_cmdFunc;
}

void MenuItem::exec() const {
  switch (itemType) {
    case SubMenu:
      item.subMenuPtr->show();
      break;
    case Command:
      *(item.command)();
      break;
    default:
      break;
  }
}
