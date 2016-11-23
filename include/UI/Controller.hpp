#ifndef TREEWITHMALE_UI_CONTROLLER_HPP_
#define TREEWITHMALE_UI_CONTROLLER_HPP_

#include <functional>
#include <memory>
#include "./UI/UI.hpp"
#include "./UI/MenuItem.hpp"
#include "./FamilyTree.hpp"

class UIController {
 public:
  static std::shared_ptr<UIController> getInstancePtr();
  void start();
  ~UIController();
 private:
  UIController();
  std::shared_ptr<UI> UIPtr;
  std::vector<std::unique_ptr<FamilyTree>> trees;
  std::size_t selectedTreeIndex = 0;
  std::size_t treesNum = 0;
  bool started = false;

  /* controllers */
  void init();
  void showCurMenu();
  void back();
  void quit();
  std::shared_ptr<Menu> createMenuPtr(const std::string &description);

  std::size_t selectTreeIndex();
  void createNewTree();
  void selectTree();
  void displayTreesInfo();

  std::shared_ptr<Menu> getOneTreeMenuPtr(const std::string &description);
  void deleteTree();

};

#endif  // TREEWITHMALE_UI_CONTROLLER_HPP_
