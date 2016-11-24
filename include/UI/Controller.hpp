#ifndef TREEWITHMALE_UI_CONTROLLER_HPP_
#define TREEWITHMALE_UI_CONTROLLER_HPP_

#include <functional>
#include <memory>
#include "./UI/UI.hpp"
#include "./UI/MenuItem.hpp"
#include "./FamilyTree.hpp"
#include "./person/Person.hpp"

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
  std::size_t selectedPersonIndex = 0;

  /* controllers */
  void init();
  void showCurMenu();
  void expandMenu(std::shared_ptr<Menu> menuPtr);
  void back();
  void quit();
  std::shared_ptr<Menu> createMenuPtr(const std::string &description, bool needsBack = true);

  void createNewTree();
  std::size_t selectTreeIndex();
  void selectTree();
  void displayTreesInfo();

  std::shared_ptr<Menu> getOneTreeMenuPtr(const std::string &description);
  std::size_t selectPersonIndex();
  void selectPerson();
  void displayTree();
  void deleteTree();

  std::shared_ptr<Menu> getOnePersonMenuPtr(const std::string &description);
  void personDie();
  void addWife();
  void addChild();

};

#endif  // TREEWITHMALE_UI_CONTROLLER_HPP_
