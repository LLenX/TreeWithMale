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
  std::vector<std::shared_ptr<FamilyTree>> trees;
  std::vector<std::size_t> treesPeopleTotal;
  std::size_t selectedTreeIndex = 0;
  std::size_t treesNum = 0;
  bool started = false;

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
  std::shared_ptr<const Person> selectPersonIndex();
  void selectPerson();
  void displayTree();
  void deleteTree();
  void searchTreeForPeopleByName();
  const std::string &getAncestorNameByTreeIndex(std::size_t index);

  std::shared_ptr<Menu> getOnePersonMenuPtr(const std::string &name, std::shared_ptr<const Person> personPtr);
  inline bool personIsMale(std::shared_ptr<const Person> personPtr);
  void die();
  void marry();
  void divorce();
  void addChild();
  void checkoutMother();
  void checkoutFather();
  void checkoutCouple();
  void checkoutSilbings();
  void checkoutChildren();
  void checkoutExWives();

};

#endif  // TREEWITHMALE_UI_CONTROLLER_HPP_
