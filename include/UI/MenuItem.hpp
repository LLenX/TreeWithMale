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
  union ItemPtr {
   public:
    friend class MenuItem;
    ItemPtr();
    ~ItemPtr();
    std::shared_ptr<Menu> subMenuPtr;
    CmdFunc commandFunc;
  };
 public:
  friend class Menu;
  MenuItem(MenuItemType i_type,
           const std::string &i_key, const std::string &i_description,
           std::shared_ptr<Menu> i_subMenuPtr,
           bool i_isShortcut = false);
  MenuItem(MenuItemType i_type,
           const std::string &i_key, const std::string &i_description,
           CmdFunc i_cmdFunc,
           bool i_isShortcut = false);
  void exec() const;
  bool isSubMenu() const;
 private:
  ItemPtr item;
  const MenuItemType type;
  const std::string key;
  const std::string description;
  const bool isShortcut;
};

#endif  // TREEWITHMALE_UI_MENUITEM_HPP_
