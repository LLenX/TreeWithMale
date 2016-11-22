#ifndef UI_MENUITEM_HPP
#define UI_MENUITEM_HPP

#include <functional>
#include <string>

class Menu;
typedef std::function<void (*)()> CmdFunc;
class MenuItem {
 private:
  union ItemPtr {
   std::shared_ptr<Menu> subMenuPtr;
   std::shared_ptr<CmdFunc> command;
  };
 public:
  enum ItemType {
    SubMenu,
    Command
  };
  MenuItem(ItemType i_itemType, const std::string &i_key, std::shared_ptr<Menu> i_subMenuPtr);
  MenuItem(ItemType i_itemType, const std::string &i_key, std::shared_ptr<CmdFunc> i_cmdFunc);
  void exec() const;
 private:
  ItemPtr item;
  const ItemType itemType;
  const std::string key;
};

#endif  // UI_MENUITEM_HPP
