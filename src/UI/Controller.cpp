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
    CREATE_COMMAND_ITEM("select", "(s) Select one tree for further operations", selectTree, false)
  );
  UIPtr->topMenuPtr->addOp(
    CREATE_COMMAND_ITEM("s", "(s) Select one tree for further operations", selectTree, true)
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
    std::cout << "[" << index + 1 << "] " << getAncestorNameByTreeIndex(index) << "'s family tree" << std::endl;
  }
}

const std::string &UIController::getAncestorNameByTreeIndex(std::size_t index) {
  return trees[index]->SelectPerson("1")->Name();
}

void UIController::createNewTree() {
  std::shared_ptr<FamilyTree> newTree(new FamilyTree());
  trees.push_back(newTree);
  std::string name = UIPtr->input([&](const std::string &inputStr) -> std::string {
    std::string ret = inputStr;
    ret.clear();
    return ret;
  }, "[Ancestor's Name]");
  newTree->CreateAncestor({"1", name});
  selectedTreeIndex = trees.size();
  ++treesNum;
  std::cout << name << "'s family tree was created successfully." << std::endl;
  std::stringstream ss;
  ss << trees.size();
  expandMenu(getOneTreeMenuPtr(name + "'s family tree"));
}

std::size_t UIController::selectTreeIndex() {
  displayTreesInfo();
  std::size_t ret = 0;
  if (treesNum == 0) {
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
      return "Family tree [" + ss.str() + "] not founded. Please try a valid index.";
    }
    ret = inputIndex;
    selectedTreeIndex = inputIndex - 1;
    return "";
  }, "[Tree Index]");
  return ret;
}

void UIController::deleteTree() {
  --treesNum;
  std::cout << "[" << selectedTreeIndex + 1 << "] " << getAncestorNameByTreeIndex(selectedTreeIndex) << "'s family tree was deleted successfully." << std::endl;
  trees[selectedTreeIndex].reset();
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
  std::cout << "[" + ss.str() + "] " << getAncestorNameByTreeIndex(selectedTreeIndex) << "'s was selected." << std::endl;
  expandMenu(getOneTreeMenuPtr(getAncestorNameByTreeIndex(selectedTreeIndex) + "'s family tree"));
}

std::shared_ptr<Menu> UIController::createMenuPtr(const std::string &description, bool needsBack) {
  auto oneMenuPtr = std::shared_ptr<Menu>(new Menu(description));
  oneMenuPtr->addOp(
    CREATE_COMMAND_ITEM("help", "(h) Show current menu", showCurMenu, false)
  );
  oneMenuPtr->addOp(
    CREATE_COMMAND_ITEM("h", "(h) Show current menu", showCurMenu, true)
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
    CREATE_COMMAND_ITEM("show", "Display " + description, displayTree, false)
  );
  oneTreeMenuPtr->addOp(
    CREATE_COMMAND_ITEM("select", "(s) Select one person for further operations", selectPerson, false)
  );
  oneTreeMenuPtr->addOp(
    CREATE_COMMAND_ITEM("s", "(s) Select one person for further operations", selectPerson, true)
  );

/* --------------------- Submenu: Search --------------------- */
  std::string searchSubMenuDescription = "Search " + description;
  auto searchSubMenuPtr = createMenuPtr(searchSubMenuDescription);
  searchSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("name", "Search for people by name", searchTreeForPeopleByName, false)
  );
  oneTreeMenuPtr->addOp(
    CREATE_SUB_MENU("search", searchSubMenuDescription, searchSubMenuPtr, false)
  );
/* ----------------------------------------------------------- */
  return oneTreeMenuPtr;
}

void UIController::displayTree() {
  std::cout << "hai mei zuo ne" << std::endl;
}

void UIController::selectPerson() {
  selectPersonIndex();
  if (selectedPersonPtr == nullptr) return;
  std::string personDescription = "[" + selectedPersonPtr->Id() + "] " + selectedPersonPtr->Name();
  std::cout << personDescription << " was selected." << std::endl;
  expandMenu(getOnePersonMenuPtr(personDescription));
}

void UIController::selectPersonIndex() {
  displayTree();
  std::cout << std::endl;
  UIPtr->input([&](const std::string &inputStr) -> std::string {
    std::ptrdiff_t inputIndex = 0;
    std::stringstream ss(inputStr);
    ss >> inputIndex;
    if (ss.fail()) {
      return "Bad Input. Please try again.";
    }
    auto onePersonPtr = trees[selectedTreeIndex]->SelectPerson(inputStr);
    if (onePersonPtr == nullptr) {
      return "Person [" + ss.str() + "] doesn't exist. Please choose an existing person.";
    }
    selectedPersonPtr.reset(const_cast<Person *>(onePersonPtr.get()));
    return "";
  }, "[Person Index]");
}

void UIController::searchTreeForPeopleByName() {
  std::cout << std::endl;
  std::vector<std::string> ids;
  UIPtr->input([&](const std::string &inputStr) -> std::string {
    ids.clear();
    trees[selectedTreeIndex]->Traverse([&](const auto &pair) -> void {
      if (pair.second->Name() != inputStr) return;
      ids.push_back(pair.second->Id());
    });
    if (ids.empty()) {
      return "There doesn't seem to be anybody called [" + inputStr + "] in this family tree. Please try another name.";
    }
    return "";
  }, "[name]");
  for (auto &oneId : ids) {
    auto onePersonPtr = trees[selectedTreeIndex]->SelectPerson(oneId);
    if (onePersonPtr == nullptr) continue;
    std::cout << "[" << oneId << "] " << onePersonPtr->Name() << std::endl;
  }
}

std::shared_ptr<Menu> UIController::getOnePersonMenuPtr(const std::string &description) {
  auto onePersonMenuPtr = createMenuPtr(description);
  onePersonMenuPtr->addOp(
    CREATE_COMMAND_ITEM("die", "Mark dead " + description, die, false)
  );
  onePersonMenuPtr->addOp(
    CREATE_COMMAND_ITEM("child", "Add a child to " + description, addChild, false)
  );

/* --------------------- Submenu: Check out relatives information --------------------- */
  std::string checkoutRalativesInfoSubMenuDescription = "Check out relatives information of " + description;
  auto checkoutRalativesInfoSubMenuPtr = createMenuPtr(checkoutRalativesInfoSubMenuDescription);
  checkoutRalativesInfoSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("mother", "(mom) Check out the mother of " + description, checkoutMother, false)
  );
  checkoutRalativesInfoSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("mom", "(mom) Check out the mother of " + description, checkoutMother, true)
  );
  checkoutRalativesInfoSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("father", "(fa) Check out the father of " + description, checkoutFather, false)
  );
  checkoutRalativesInfoSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("fa", "(fa) Check out the father of " + description, checkoutFather, true)
  );
  checkoutRalativesInfoSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("wife", "(wi) Check out the wife of " + description, checkoutCouple, false)
  );
  checkoutRalativesInfoSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("wi", "(wi) Check out the wife of " + description, checkoutCouple, true)
  );
  checkoutRalativesInfoSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("sibling", "(si) Check out the siblings of " + description, checkoutSilbings, false)
  );
  checkoutRalativesInfoSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("si", "(si) Check out the siblings of " + description, checkoutSilbings, true)
  );
  checkoutRalativesInfoSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("children", "(ch) Check out the children of " + description, checkoutChildren, false)
  );
  checkoutRalativesInfoSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("ch", "(ch) Check out the children of " + description, checkoutChildren, true)
  );

  onePersonMenuPtr->addOp(
    CREATE_SUB_MENU("check", "(c) " + checkoutRalativesInfoSubMenuDescription, checkoutRalativesInfoSubMenuPtr, false)
  );
  onePersonMenuPtr->addOp(
    CREATE_SUB_MENU("c", "(c) " + checkoutRalativesInfoSubMenuDescription, checkoutRalativesInfoSubMenuPtr, true)
  );
/* ------------------------------------------------------------------ */

/* --------------------- Submenu: Marriage management --------------------- */
  std::string marriageOpDescription = "Marriage management on " + description;
  auto marriageOpSubMenuPtr = createMenuPtr(marriageOpDescription);
  marriageOpSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("wife", "(wi) Check out the wife of " + description, checkoutCouple, false)
  );
  marriageOpSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("wi", "(wi) Check out the wife of " + description, checkoutCouple, true)
  );
  marriageOpSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("exwife", "(ex) Check out the exwives of " + description, checkoutExWives, false)
  );
  marriageOpSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("ex", "(ex) Check out the exwives of " + description, checkoutExWives, true)
  );
  marriageOpSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("marry", "Add information about the woman married by " + description, marry, false)
  );
  marriageOpSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("divorce", "Mark divorced " + description, divorce, false)
  );

  onePersonMenuPtr->addOp(
    CREATE_SUB_MENU("marriage", "(m) " + marriageOpDescription, marriageOpSubMenuPtr, false)
  );
  onePersonMenuPtr->addOp(
    CREATE_SUB_MENU("m", "(m) " + marriageOpDescription, marriageOpSubMenuPtr, true)
  );
/* ------------------------------------------------------------------ */

  return onePersonMenuPtr;
}

void UIController::die() {
  std::cout << "die: hai mei zuo ne" << std::endl;
}
void UIController::marry() {
  std::cout << "marry: hai mei zuo ne" << std::endl;
}
void UIController::divorce() {
  std::cout << "divorce: hai mei zuo ne" << std::endl;
}
void UIController::addChild() {
  std::cout << "addChild: hai mei zuo ne" << std::endl;
}
void UIController::checkoutMother() {
  std::cout << "checkoutMother: hai mei zuo ne" << std::endl;
}
void UIController::checkoutFather() {
  std::cout << "checkoutFather: hai mei zuo ne" << std::endl;
}
void UIController::checkoutCouple() {
  std::cout << "checkoutCouple: hai mei zuo ne" << std::endl;
}
void UIController::checkoutSilbings() {
  std::cout << "checkoutSilbings: hai mei zuo ne" << std::endl;
}
void UIController::checkoutChildren() {
  std::cout << "checkoutChildren: hai mei zuo ne" << std::endl;
}
void UIController::checkoutExWives() {
  std::cout << "checkoutExWives: hai mei zuo ne" << std::endl;
}
