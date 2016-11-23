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

#define CREATE_MENU_ITEM(type, op, description, func, isShortcut) (std::shared_ptr<MenuItem>(new MenuItem(type, op, description, [&]() -> void { \
  func(); \
}, isShortcut)))

UIController::UIController()
: UIPtr(UI::getInstancePtr()) {
  
  UIPtr->topMenuPtr->addOp(
    CREATE_MENU_ITEM(Command, "new", "Create a Tree", createNewTree, false)
  );
  UIPtr->topMenuPtr->addOp(
    CREATE_MENU_ITEM(Command, "select", "select One Tree", selectTree, false)
  );
  UIPtr->topMenuPtr->addOp(
    CREATE_MENU_ITEM(Command, "info", "Display Infomation of Trees", displayTreesInfo, false)
  );
  UIPtr->topMenuPtr->addOp(
    CREATE_MENU_ITEM(Command, "help", "(h) Show Current Menu", showCurMenu, false)
  );
  UIPtr->topMenuPtr->addOp(
    CREATE_MENU_ITEM(Command, "h", "(h) Show Current Menu", showCurMenu, true)
  );
  UIPtr->topMenuPtr->addOp(
    CREATE_MENU_ITEM(Command, "quit", "(q) Quit", quit, false)
  );
  UIPtr->topMenuPtr->addOp(
    CREATE_MENU_ITEM(Command, "q", "(q) Quit", quit, true)
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
    std::cout << "[" << index + 1 << "] " << "toaru tree" << std::endl;
  }
}

void UIController::createNewTree() {
  trees.push_back(std::make_unique<FamilyTree>());
  ++treesNum;
  std::cout << "Family Tree #" << trees.size() << " was created successfully." << std::endl;
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
      return "Family Tree #" + ss.str() + " not found. Please try a valid index.";
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
  std::cout << "Family Tree #" << selectedTreeIndex + 1 << " was deleted successfully." << std::endl;
  selectedTreeIndex = 0;
  std::cout << std::endl;
  back();
}

void UIController::selectTree() {
  auto index = selectTreeIndex();
  if (index == 0) return;
  std::stringstream ss;
  ss << index;
  std::cout << "Family Tree #" + ss.str() + " was selected." << std::endl;
  auto oneTreeMenuPtr = getOneTreeMenuPtr("Family Tree #" + ss.str());
  UIPtr->menuStack.push(oneTreeMenuPtr);
  UIPtr->curMenuPtr = oneTreeMenuPtr;
  std::cout << std::endl;
  showCurMenu();
}

std::shared_ptr<Menu> UIController::createMenuPtr(const std::string &description) {
  auto oneMenuPtr = std::shared_ptr<Menu>(new Menu(description));
  oneMenuPtr->addOp(
    CREATE_MENU_ITEM(Command, "help", "(h) Show Current Menu", showCurMenu, false)
  );
  oneMenuPtr->addOp(
    CREATE_MENU_ITEM(Command, "h", "(h) Show Current Menu", showCurMenu, true)
  );
  oneMenuPtr->addOp(
    CREATE_MENU_ITEM(Command, "back", "(b) Back", back, false)
  );
  oneMenuPtr->addOp(
    CREATE_MENU_ITEM(Command, "quit", "(q) Quit", quit, false)
  );
  return oneMenuPtr;
}

std::shared_ptr<Menu> UIController::getOneTreeMenuPtr(const std::string &description) {
  auto oneTreeMenuPtr = createMenuPtr(description);
  oneTreeMenuPtr->addOp(
    CREATE_MENU_ITEM(Command, "delete", "Delete " + description, deleteTree, false)
  );
  return oneTreeMenuPtr;
}
