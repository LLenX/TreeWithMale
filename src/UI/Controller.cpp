#include <memory>
#include <iostream>
#include <sstream>
#include "./FamilyTree.hpp"
#include "./UI/MenuItem.hpp"
#include "./UI/Menu.hpp"
#include "./UI/UI.hpp"
#include "./UI/Controller.hpp"

using MenuItemType::Command;
using MenuItemType::SubMenu;

#define CREATE_COMMAND_ITEM(op, description, func, isShortcut) (std::shared_ptr<MenuItem>(new MenuItem(Command, op, description, [&]() -> void { \
  func(); \
}, isShortcut)))

#define CREATE_SUB_MENU(op, description, SubMenuPtr, isShortcut) (std::shared_ptr<MenuItem>(new MenuItem(SubMenu, op, description, SubMenuPtr, isShortcut)))

UIController::UIController()
: UIPtr(UI::getInstancePtr()) {
  UIPtr->topMenuPtr = createMenuPtr("Main", false);
  UIPtr->curMenuPtr = UIPtr->topMenuPtr;
  UIPtr->topMenuPtr->addOp(
    CREATE_COMMAND_ITEM("new", "Create a tree", createNewTree, false)
  );
  UIPtr->topMenuPtr->addOp(
    CREATE_COMMAND_ITEM("select", "Select one tree for further operations", selectTree, false)
  );
  
}

UIController::~UIController() {}

std::shared_ptr<UIController> UIController::getInstancePtr() {
  static std::shared_ptr<UIController> instancePtr(new UIController());
  return instancePtr;
}

void UIController::start() {
  init();
  std::cout << std::endl;
  showCurMenu();
  while (started) {
    std::cout << std::endl;
    UIPtr->oneLoop();
  }
}

/* controllers */

void UIController::init() {
  started = true;
  UIPtr->menuStack.push(UIPtr->topMenuPtr);
}

void UIController::quit() {
  started = false;
  std::cout << "Goodbye Kugou!\n" << std::endl;
}

void UIController::back() {
  if (UIPtr->menuStack.size() > 1) {
    UIPtr->menuStack.pop();
    UIPtr->curMenuPtr = UIPtr->menuStack.top();
  }
  showCurMenu();
}

void UIController::showCurMenu() {
  UIPtr->curMenuPtr->show();
}

void UIController::displayTreesInfo() {
  if (trees.empty()) {
    std::cout << "No trees available." << std::endl;
    return;
  }
  for (std::size_t index = 0, size = trees.size(); index < size; ++index) {
    if (trees[index].get() == nullptr) continue;
    std::cout << "tree" << " sono [" << index + 1 << "] " << std::endl;
  }
}

void UIController::createNewTree() {
  trees.push_back(std::make_unique<FamilyTree>());
  ++treesNum;
  std::cout << "Family Tree [" << trees.size() << "] was created successfully." << std::endl;
}

std::size_t UIController::selectTreeIndex() {
  displayTreesInfo();
  std::size_t ret = 0;
  if (trees.empty()) {
    return ret;
  }
  std::cout << std::endl;
  UIPtr->input([&](const std::string &inputStr) -> std::string {
    std::ptrdiff_t inputIndex = 0;
    std::stringstream ss(inputStr);
    ss >> inputIndex;
    if (ss.fail()) {
      return "Bad Input. Please try again.";
    } else if (inputIndex < 1
               or static_cast<std::ptrdiff_t>(trees.size()) < inputIndex
               or trees[inputIndex - 1].get() == nullptr) {
      ss.clear();
      ss << inputIndex;
      return "Family Tree [" + ss.str() + "] not founded. Please try a valid index.";
    }
    ret = inputIndex;
    selectedTreeIndex = inputIndex - 1;
    return "";
  }, "[Tree Index] ");
  return ret;
}

void UIController::deleteTree() {
  trees[selectedTreeIndex].reset(nullptr);
  --treesNum;
  if (treesNum == 0) {
    trees.clear();
  }
  std::cout << "Family Tree [" << selectedTreeIndex + 1 << "] was deleted successfully." << std::endl;
  selectedTreeIndex = 0;
  std::cout << std::endl;
  back();
}

void UIController::expandMenu(std::shared_ptr<Menu> menuPtr) {
  UIPtr->menuStack.push(menuPtr);
  UIPtr->curMenuPtr = menuPtr;
  std::cout << std::endl;
  showCurMenu();
}

void UIController::selectTree() {
  auto index = selectTreeIndex();
  if (index == 0) return;
  std::stringstream ss;
  ss << index;
  std::cout << "Family Tree [" + ss.str() + "] was selected." << std::endl;
  expandMenu(getOneTreeMenuPtr("Family Tree [" + ss.str() + "]"));
}

std::shared_ptr<Menu> UIController::createMenuPtr(const std::string &description, bool needsBack) {
  auto oneMenuPtr = std::shared_ptr<Menu>(new Menu(description));
  oneMenuPtr->addOp(
    CREATE_COMMAND_ITEM("help", "(h) Show Current Menu", showCurMenu, false)
  );
  oneMenuPtr->addOp(
    CREATE_COMMAND_ITEM("h", "(h) Show Current Menu", showCurMenu, true)
  );
  oneMenuPtr->addOp(
    CREATE_COMMAND_ITEM("quit", "(q) Quit", quit, false)
  );
  oneMenuPtr->addOp(
    CREATE_COMMAND_ITEM("q", "(q) Quit", quit, true)
  );
  if (needsBack) {
    oneMenuPtr->addOp(
      CREATE_COMMAND_ITEM("back", "(b) Back", back, false)
    );
    oneMenuPtr->addOp(
      CREATE_COMMAND_ITEM("b", "(b) Back", back, true)
    );
  }
  return oneMenuPtr;
}

std::shared_ptr<Menu> UIController::getOneTreeMenuPtr(const std::string &description) {
  auto oneTreeMenuPtr = createMenuPtr(description);
  oneTreeMenuPtr->addOp(
    CREATE_COMMAND_ITEM("delete", "Delete " + description, deleteTree, false)
  );
  oneTreeMenuPtr->addOp(
    CREATE_COMMAND_ITEM("select", "Select one person for further operations", selectPerson, false)
  );
  return oneTreeMenuPtr;
}

void UIController::displayTree() {
  std::cout << "hai mei zuo ne" << std::endl;
}

void UIController::selectPerson() {
  auto index = selectPersonIndex();
  if (index == 0) return;
  std::stringstream ss;
  ss << index;
  std::cout << "Person [" + ss.str() + "] was selected." << std::endl;
  expandMenu(getOnePersonMenuPtr("Person [" + ss.str() + "]"));
}

std::size_t UIController::selectPersonIndex() {
  displayTree();
  std::size_t ret = 0;
  // if (trees.empty()) {
  //   return ret;
  // }
  std::cout << std::endl;
  UIPtr->input([&](const std::string &inputStr) -> std::string {
    std::ptrdiff_t inputIndex = 0;
    std::stringstream ss(inputStr);
    ss >> inputIndex;
    if (ss.fail()) {
      return "Bad Input. Please try again.";
    // } else if (inputIndex < 1
    //            or static_cast<std::ptrdiff_t>(trees.size()) < inputIndex
    //            or trees[inputIndex - 1].get() == nullptr) {
    //   ss.clear();
    //   ss << inputIndex;
    //   return "Person [" + ss.str() + "] doesn't exist. Please choose an existing person.";
    }
    ret = inputIndex;
    selectedPersonIndex = inputIndex - 1;
    return "";
  }, "[Person Index] ");
  return ret;
}

std::shared_ptr<Menu> UIController::getOnePersonMenuPtr(const std::string &description) {
  auto onePersonMenuPtr = createMenuPtr(description);
  onePersonMenuPtr->addOp(
    CREATE_COMMAND_ITEM("die", "Mark " + description + " is dead", personDie, false)
  );
  std::string addRalativeSubMenuDescription = "Add a relative to " + description;
  auto addRalativeSubMenuPtr = createMenuPtr(addRalativeSubMenuDescription);
  addRalativeSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("wife", "Add a wife to " + description, addWife, false)
  );
  addRalativeSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("child", "Add a child to " + description, addChild, false)
  );
  onePersonMenuPtr->addOp(
    CREATE_SUB_MENU("add", addRalativeSubMenuDescription, addRalativeSubMenuPtr, false)
  );
  onePersonMenuPtr->addOp(
    CREATE_SUB_MENU("a", addRalativeSubMenuDescription, addRalativeSubMenuPtr, true)
  );
  return onePersonMenuPtr;
}

void UIController::personDie() {
  std::cout << "I am to commit suicide." << std::endl;
}

void UIController::addWife() {
  std::cout << "Bie xiang le, single dog" << std::endl;
}

void UIController::addChild() {
  std::cout << "Lian Lao po dou meiyou" << std::endl;
}

