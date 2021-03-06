#include <memory>
#include <iostream>
#include <sstream>
#include <signal.h>
#include "./FamilyTree.hpp"
#include "./utility/PrintTree.hpp"
#include "./UI/MenuItem.hpp"
#include "./UI/Menu.hpp"
#include "./UI/UI.hpp"
#include "./UI/Controller.hpp"
#include "./UI/Terminal.hpp"

using MenuItemType::Command;
using MenuItemType::SubMenu;

#define CREATE_COMMAND_ITEM(op, description, func, isShortcut) (std::shared_ptr<MenuItem>(new MenuItem(Command, op, description, [&]() -> void { \
  func(); \
}, isShortcut)))

#define CREATE_CONTROL_ITEM(op, description, func, isShortcut) (std::shared_ptr<MenuItem>(new MenuItem(Command, op, description, [&]() -> void { \
  func(); \
}, isShortcut, true)))

#define CREATE_SUB_MENU(op, description, SubMenuPtr, isShortcut) (std::shared_ptr<MenuItem>(new MenuItem(SubMenu, op, description, SubMenuPtr, isShortcut)))

void gracefullyExit(int sig);

UIController::UIController()
: UIPtr(UI::getInstancePtr()) {
  UIPtr->topMenuPtr = createMenuPtr("Main", false);
  UIPtr->curMenuPtr = UIPtr->topMenuPtr;
  UIPtr->topMenuPtr->addOp(
    CREATE_COMMAND_ITEM("new", "(n) Create a tree", createNewTree, false)
  );
  UIPtr->topMenuPtr->addOp(
    CREATE_COMMAND_ITEM("n", "(n) Create a tree", createNewTree, true)
  );
  UIPtr->topMenuPtr->addOp(
    CREATE_COMMAND_ITEM("select", "(s) Select one tree for further operations", selectTree, false)
  );
  UIPtr->topMenuPtr->addOp(
    CREATE_COMMAND_ITEM("s", "(s) Select one tree for further operations", selectTree, true)
  );
  signal(SIGSEGV, gracefullyExit);
}

UIController::~UIController() {}

std::shared_ptr<UIController> UIController::getInstancePtr() {
  static std::shared_ptr<UIController> instancePtr(new UIController());
  return instancePtr;
}

void UIController::resume() {
  std::cout << std::endl;
  showCurMenu();
  while (started) {
    std::cout << std::endl;
    UIPtr->oneLoop();
  }
}

void UIController::start() {
  init();
  resume();
}

/* controllers */

void UIController::init() {
  started = true;
  UIPtr->menuStack.push(UIPtr->topMenuPtr);
}

void UIController::quit() {
  started = false;
  std::cout << "Goodbye!\n" << std::endl;
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
    std::cout << "[" << index + 1 << "] "
              << getAncestorNameByTreeIndex(index)
              << "'s family tree"
              << std::endl;
  }
}

const std::string &UIController::getAncestorNameByTreeIndex(std::size_t index) {
  auto original = trees[index]->SelectPerson();
  const auto &name = trees[index]->SelectPerson("1")->Name();
  trees[index]->SelectPerson(original->Id());
  return name;
}

void UIController::createNewTree() {
  std::string name = UIPtr->input([&](const std::string &inputStr) -> std::string {
    std::string ret = inputStr;
    ret.clear();
    return ret;
  }, "[Ancestor's Name]");

  std::shared_ptr<FamilyTree> newTree(new FamilyTree());
  selectedTreeIndex = trees.size();
  trees.push_back(newTree);
  treesPeopleTotal.push_back(1);
  newTree->CreateAncestor(Person::Info("1", name));
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
  std::cout << "[" << selectedTreeIndex + 1 << "] "
            << getAncestorNameByTreeIndex(selectedTreeIndex)
            << "'s family tree was deleted successfully."
            << std::endl;
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
  std::cout << "[" + ss.str() + "] "
            << getAncestorNameByTreeIndex(selectedTreeIndex)
            << "'s family tree is selected." << std::endl;
  expandMenu(getOneTreeMenuPtr(getAncestorNameByTreeIndex(selectedTreeIndex) + "'s family tree"));
}

std::shared_ptr<Menu> UIController::createMenuPtr(const std::string &description, bool needsBack) {
  auto oneMenuPtr = std::shared_ptr<Menu>(new Menu(description));
  oneMenuPtr->addOp(
    CREATE_CONTROL_ITEM("help", "(h) Show current menu", showCurMenu, false)
  );
  oneMenuPtr->addOp(
    CREATE_CONTROL_ITEM("h", "(h) Show current menu", showCurMenu, true)
  );
  oneMenuPtr->addOp(
    CREATE_CONTROL_ITEM("quit", "(q) Quit", quit, false)
  );
  oneMenuPtr->addOp(
    CREATE_CONTROL_ITEM("q", "(q) Quit", quit, true)
  );
  if (needsBack) {
    oneMenuPtr->addOp(
      CREATE_CONTROL_ITEM("back", "(b) Back", back, false)
    );
    oneMenuPtr->addOp(
      CREATE_CONTROL_ITEM("b", "(b) Back", back, true)
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
  printPersonTree(trees[selectedTreeIndex]->SelectPerson("1"));
}

void UIController::selectPerson() {
  auto selectedPersonPtr = selectPersonIndex();
  if (selectedPersonPtr == nullptr) return;
  std::string name = selectedPersonPtr->Name();
  std::cout << "[" + selectedPersonPtr->Id() + "] "
            << name
            << " is selected."
            << std::endl;
  expandMenu(getOnePersonMenuPtr(name, selectedPersonPtr));
}

std::shared_ptr<const Person> UIController::selectPersonIndex() {
  displayTree();
  std::cout << "Please select an index from:" << std::endl;
  trees[selectedTreeIndex]->Traverse([&](const auto &onePersonPtr) -> void {
    std::cout << "[" + onePersonPtr->Id() + "] "
              << onePersonPtr->Name()
              << std::endl;
  });
  std::cout << std::endl;
  std::shared_ptr<const Person> selectedPersonPtr(nullptr);
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
    selectedPersonPtr = onePersonPtr;
    return "";
  }, "[Person Index]");
  return selectedPersonPtr;
}

void UIController::searchTreeForPeopleByName() {
  std::vector<std::string> ids;
  UIPtr->input([&](const std::string &inputStr) -> std::string {
    ids.clear();
    trees[selectedTreeIndex]->Traverse([&](const auto &onePersonPtr) -> void {
      if (onePersonPtr->Name() != inputStr) return;
      ids.push_back(onePersonPtr->Id());
    });
    if (ids.empty()) {
      std::cout << "There doesn't seem to be anybody called [" + inputStr + "] in this family tree." << std::endl;
    }
    return "";
  }, "[name]");
  for (auto &oneId : ids) {
    auto onePersonPtr = trees[selectedTreeIndex]->SelectPerson(oneId);
    if (onePersonPtr == nullptr) continue;
    std::cout << "[" + oneId + "] "
              << onePersonPtr->Name()
              << std::endl;
  }
}

inline bool UIController::personIsMale(std::shared_ptr<const Person> personPtr) {
  return personPtr->Gender() == Person::PersonGender::MALE;
}

std::shared_ptr<Menu> UIController::getOnePersonMenuPtr(const std::string &name, std::shared_ptr<const Person> personPtr) {
  auto onePersonMenuPtr = createMenuPtr(name);
  onePersonMenuPtr->addOp(
    CREATE_COMMAND_ITEM("die", "(d) Mark " + name + " dead", die, false)
  );
  onePersonMenuPtr->addOp(
    CREATE_COMMAND_ITEM("d", "(d) Mark " + name + " dead", die, true)
  );
  onePersonMenuPtr->addOp(
    CREATE_COMMAND_ITEM("addch", "(a) Add a child to " + name, addChild, false)
  );
  onePersonMenuPtr->addOp(
    CREATE_COMMAND_ITEM("a", "(a) Add a child to " + name, addChild, true)
  );

/* --------------------- Submenu: Check out relatives information --------------------- */
  std::string checkoutRalativesInfoSubMenuDescription = "Check out relatives of " + name;
  auto checkoutRalativesInfoSubMenuPtr = createMenuPtr(checkoutRalativesInfoSubMenuDescription);
  checkoutRalativesInfoSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("mother", "(mo) Check out the mother of " + name, checkoutMother, false)
  );
  checkoutRalativesInfoSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("mo", "(mo) Check out the mother of " + name, checkoutMother, true)
  );
  checkoutRalativesInfoSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("father", "(fa) Check out the father of " + name, checkoutFather, false)
  );
  checkoutRalativesInfoSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("fa", "(fa) Check out the father of " + name, checkoutFather, true)
  );
  if (personIsMale(personPtr)) {
    checkoutRalativesInfoSubMenuPtr->addOp(
      CREATE_COMMAND_ITEM("wife", "(wi) Check out the wife of " + name, checkoutCouple, false)
    );
    checkoutRalativesInfoSubMenuPtr->addOp(
      CREATE_COMMAND_ITEM("wi", "(wi) Check out the wife of " + name, checkoutCouple, true)
    );
  } else {
    checkoutRalativesInfoSubMenuPtr->addOp(
      CREATE_COMMAND_ITEM("husband", "(hs) Check out the husband of " + name, checkoutCouple, false)
    );
    checkoutRalativesInfoSubMenuPtr->addOp(
      CREATE_COMMAND_ITEM("hs", "(hs) Check out the husband of " + name, checkoutCouple, true)
    );
  }
  
  checkoutRalativesInfoSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("siblings", "(si) Check out the siblings of " + name, checkoutSilbings, false)
  );
  checkoutRalativesInfoSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("si", "(si) Check out the siblings of " + name, checkoutSilbings, true)
  );
  checkoutRalativesInfoSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("children", "(ch) Check out the children of " + name, checkoutChildren, false)
  );
  checkoutRalativesInfoSubMenuPtr->addOp(
    CREATE_COMMAND_ITEM("ch", "(ch) Check out the children of " + name, checkoutChildren, true)
  );

  onePersonMenuPtr->addOp(
    CREATE_SUB_MENU("check", "(c) " + checkoutRalativesInfoSubMenuDescription, checkoutRalativesInfoSubMenuPtr, false)
  );
  onePersonMenuPtr->addOp(
    CREATE_SUB_MENU("c", "(c) " + checkoutRalativesInfoSubMenuDescription, checkoutRalativesInfoSubMenuPtr, true)
  );
/* ------------------------------------------------------------------ */

/* --------------------- Submenu: Marriage management --------------------- */
  std::string marriageOpDescription = "Marriage management on " + name;
  auto marriageOpSubMenuPtr = createMenuPtr(marriageOpDescription);
  if (personIsMale(personPtr)) {
    checkoutRalativesInfoSubMenuPtr->addOp(
      CREATE_COMMAND_ITEM("wife", "(wi) Check out the wife of " + name, checkoutCouple, false)
    );
    checkoutRalativesInfoSubMenuPtr->addOp(
      CREATE_COMMAND_ITEM("wi", "(wi) Check out the wife of " + name, checkoutCouple, true)
    );
    marriageOpSubMenuPtr->addOp(
      CREATE_COMMAND_ITEM("exwife", "(ex) Check out the exwives of " + name, checkoutExWives, false)
    );
    marriageOpSubMenuPtr->addOp(
      CREATE_COMMAND_ITEM("ex", "(ex) Check out the exwives of " + name, checkoutExWives, true)
    );
    marriageOpSubMenuPtr->addOp(
      CREATE_COMMAND_ITEM("marry", "(m) Add information about the woman married by " + name, marry, false)
    );
    marriageOpSubMenuPtr->addOp(
      CREATE_COMMAND_ITEM("m", "(m) Add information about the woman married by " + name, marry, true)
    );
  } else {
    checkoutRalativesInfoSubMenuPtr->addOp(
      CREATE_COMMAND_ITEM("husband", "(hs) Check out the husband of " + name, checkoutCouple, false)
    );
    checkoutRalativesInfoSubMenuPtr->addOp(
      CREATE_COMMAND_ITEM("hs", "(hs) Check out the husband of " + name, checkoutCouple, true)
    );
  }
  auto original = trees[selectedTreeIndex]->SelectPerson();
  trees[selectedTreeIndex]->SelectPerson(personPtr->Id());
  if (trees[selectedTreeIndex]->GetCouple()) {
    marriageOpSubMenuPtr->addOp(
      CREATE_COMMAND_ITEM("divorce", "(d) Mark " + name + " divorced", divorce, false)
    );
    marriageOpSubMenuPtr->addOp(
      CREATE_COMMAND_ITEM("d", "(d) Mark " + name + " divorced", divorce, true)
    );
  }
  trees[selectedTreeIndex]->SelectPerson(original->Id());

  onePersonMenuPtr->addOp(
    CREATE_SUB_MENU("marriage", "(m) " + marriageOpDescription, marriageOpSubMenuPtr, false)
  );
  onePersonMenuPtr->addOp(
    CREATE_SUB_MENU("m", "(m) " + marriageOpDescription, marriageOpSubMenuPtr, true)
  );
/* ------------------------------------------------------------------ */
  onePersonMenuPtr->addOp(
    CREATE_COMMAND_ITEM("show", "Display the subtree rooted at " + marriageOpDescription, displaySubTree, false)
  );
  return onePersonMenuPtr;
}

void UIController::die() {
  auto selectedPersonPtr = trees[selectedTreeIndex]->SelectPerson();
  if (not UIPtr->confirm("Marking his death is non-invertible. Type y to confirm or other characters to quit.")) {
    return;
  }
  bool success = trees[selectedTreeIndex]->Die();
  if (success) {
    std::cout << "OK, "
              << selectedPersonPtr->Name()
              << " is now marked dead. RIP"
              << std::endl;
    return;
  }
  auto error = trees[selectedTreeIndex]->GetError();
  if (FamilyTree::Error::WRONG_LOGIC == error) {
    std::cout << selectedPersonPtr->Name()
              << " has been marked dead. Give "
              << (personIsMale(selectedPersonPtr) ? "him" : "her")
              << " a break!"
              << std::endl;
  } else {
    std::cerr << "Unexpected Die Error" << std::endl;
  }
}
void UIController::marry() {
  auto selectedPersonPtr = trees[selectedTreeIndex]->SelectPerson();
  std::cout << "Please input the wife's name" << std::endl;
  std::string wifeName = UIPtr->input([&](const std::string &inputStr) -> std::string {
    return "";
  }, "[wife's name]");
  std::stringstream ss;
  ss << treesPeopleTotal[selectedTreeIndex] + 1;
  auto wifePtr = trees[selectedTreeIndex]->Marry(Person::Info(ss.str(), wifeName));
  if (wifePtr) {
    ++treesPeopleTotal[selectedTreeIndex];
    std::cout << wifePtr->Name()
              << " is recorded as married to "
              << selectedPersonPtr->Name() << "."
              << std::endl;
    return;
  }
  auto error = trees[selectedTreeIndex]->GetError();
  if (FamilyTree::Error::WRONG_LOGIC == error) {
    std::cout << selectedPersonPtr->Name()
              << " is married. He would not like to be a second Makoto kun in School Days!"
              << std::endl;
  } else {
    std::cerr << "Unexpected Marry Error" << std::endl;
  }
}
void UIController::divorce() {
  auto selectedPersonPtr = trees[selectedTreeIndex]->SelectPerson();
  auto counterpartPtr = trees[selectedTreeIndex]->GetCouple();
  if (counterpartPtr == nullptr) {
    std::cout << "Single dog should get married first to divorce." << std::endl;
    return;
  }
  if (not UIPtr->confirm("Divorcing " + counterpartPtr->Name() + " is non-invertible. Type y to confirm or other characters to quit.")) {
    return;
  }
  bool success = trees[selectedTreeIndex]->Divorce();
  if (success) {
    std::cout << "Divorced " + counterpartPtr->Name() + " successfully." << std::endl;
    return;
  }
  auto error = trees[selectedTreeIndex]->GetError();
  if (FamilyTree::Error::INVALID_TYPE == error) {
    std::cout << selectedPersonPtr->Name()
              << " is not a parent."
              << std::endl;
  } else if (FamilyTree::Error::WRONG_LOGIC == error) {
    std::cout << "Single dog should get married first to divorce." << std::endl;
  } else {
    std::cerr << "Unexpected Divorce Error" << std::endl;
  }
}
void UIController::addChild() {
  auto selectedPersonPtr = trees[selectedTreeIndex]->SelectPerson();
  auto counterpartPtr = trees[selectedTreeIndex]->GetCouple();
  if (counterpartPtr == nullptr) {
    std::cout << "Single dog should get married first to add child." << std::endl;
    return;
  }
  std::cout << "Please input the child's name" << std::endl;
  std::string childName = UIPtr->input([&](const std::string &inputStr) -> std::string {
    return "";
  }, "[child's name]");
  std::cout << "Please input the child's gender, f for female and m for male" << std::endl;
  std::string childGender = UIPtr->input([&](const std::string &inputStr) -> std::string {
    if (inputStr != "f" and inputStr != "m") {
      return "Please answer f or m";
    }
    return "";
  }, "[gender(f/m)]");
  auto gender = (childGender == "f" ? Person::PersonGender::FEMALE : Person::PersonGender::MALE);
  std::stringstream ss;
  ss << treesPeopleTotal[selectedTreeIndex] + 1;
  auto childPtr = trees[selectedTreeIndex]->GiveBirthTo(Person::Info(ss.str(), childName), gender);
  if (childPtr) {
    ++treesPeopleTotal[selectedTreeIndex];
    std::cout << childPtr->Name()
              << " is recorded as the child of "
              << selectedPersonPtr->Name() << "."
              << std::endl;
    return;
  }
  auto error = trees[selectedTreeIndex]->GetError();
  if (FamilyTree::Error::INVALID_TYPE == error) {
    std::cout << selectedPersonPtr->Name()
              << " is not a parent."
              << std::endl;
  } else if (FamilyTree::Error::WRONG_LOGIC == error) {
    std::cout << "Single dog should get married first to divorce." << std::endl;
  } else {
    std::cerr << "Unexpected Add child Error" << std::endl;
  }
}
void UIController::checkoutMother() {
  auto selectedPersonPtr = trees[selectedTreeIndex]->SelectPerson();
  auto error = trees[selectedTreeIndex]->GetError();
  try {
    auto motherPtr = trees[selectedTreeIndex]->GetMother();
    if (motherPtr) {
      std::cout << "Mother: [" << motherPtr->Id() << "] " << motherPtr->Name() << std::endl;
      return;
    }
    error = trees[selectedTreeIndex]->GetError();
  } catch (...) {
    error = FamilyTree::Error::RESULT_NOT_FOUND;
  }
  if (FamilyTree::Error::RESULT_NOT_FOUND == error) {
    std::cout << "Mother not found due to being the root (ancestor)." << std::endl;
  } else if (FamilyTree::Error::INVALID_TYPE == error) {
    std::cout << "Mother not found due to have no blood relation with the family tree." << std::endl;
  } else {
    std::cerr << "Unexpected Checkout Mother Error" << std::endl;
  }
}
void UIController::checkoutFather() {
  auto selectedPersonPtr = trees[selectedTreeIndex]->SelectPerson();
  auto error = trees[selectedTreeIndex]->GetError();
  try {
    auto fatherPtr = trees[selectedTreeIndex]->GetFather();
    if (fatherPtr) {
      std::cout << "Father: [" << fatherPtr->Id() << "] " << fatherPtr->Name() << std::endl;
      return;
    }
    error = trees[selectedTreeIndex]->GetError();
  } catch (...) {
    error = FamilyTree::Error::RESULT_NOT_FOUND;
  }
  if (FamilyTree::Error::RESULT_NOT_FOUND == error) {
    std::cout << "Father not found due to being the root (ancestor)." << std::endl;
  } else if (FamilyTree::Error::INVALID_TYPE == error) {
    std::cout << "Father not found due to lack of blood relation with the family tree." << std::endl;
  } else {
    std::cerr << "Unexpected Checkout Father Error" << std::endl;
  }
}
void UIController::checkoutCouple() {
  auto selectedPersonPtr = trees[selectedTreeIndex]->SelectPerson();
  auto error = trees[selectedTreeIndex]->GetError();
  try {
    auto counterpartPtr = trees[selectedTreeIndex]->GetCouple();
    if (counterpartPtr) {
      std::cout << (personIsMale(counterpartPtr) ? "Husband" : "Wife")
                << ": [" + counterpartPtr->Id() + "] "
                << counterpartPtr->Name()
                << std::endl;
      return;
    }
    error = trees[selectedTreeIndex]->GetError();
  } catch (...) {
    error = FamilyTree::Error::INVALID_TYPE;
  }
  if (FamilyTree::Error::INVALID_TYPE == error) {
    std::cout << selectedPersonPtr->Name() << " is not a parent." << std::endl;
  } else {
    std::cerr << "Unexpected Checkout Couple Error" << std::endl;
  }
}
void UIController::checkoutSilbings() {
  auto selectedPersonPtr = trees[selectedTreeIndex]->SelectPerson();
  auto error = trees[selectedTreeIndex]->GetError();
  try {
    auto siblingsPtr = trees[selectedTreeIndex]->GetSiblings();
    if (siblingsPtr) {
      if (siblingsPtr->empty()) {
        std::cout << selectedPersonPtr->Name() << " is the only child." << std::endl;
      } else {
        std::cout << "Siblings:" << std::endl;
        for (auto &one : *siblingsPtr) {
          std::cout << "[" << one->Id() << "] " << one->Name() << std::endl;
        }
      }
      return;
    }
    error = trees[selectedTreeIndex]->GetError();
  } catch (...) {
    error = FamilyTree::Error::RESULT_NOT_FOUND;
  }
  if (FamilyTree::Error::INVALID_TYPE == error) {
    std::cout << "Siblings not found due to lack of blood relation with the family tree." << std::endl;
  } else if (FamilyTree::Error::RESULT_NOT_FOUND == error) {
    std::cout << "Siblings not found due to being the root (ancestor)." << std::endl;
  } else {
    std::cerr << "Unexpected Checkout Siblings Error" << std::endl;
  }
}
void UIController::checkoutChildren() {
  auto selectedPersonPtr = trees[selectedTreeIndex]->SelectPerson();
  auto error = trees[selectedTreeIndex]->GetError();
  try {
    auto childrenPtr = trees[selectedTreeIndex]->GetChildren();
    if (childrenPtr) {
      if (childrenPtr->empty()) {
        std::cout << selectedPersonPtr->Name() << " has no children for the moment." << std::endl;
      } else {
        std::cout << (childrenPtr->size() == 1 ? "The only child:" : "Children:") << std::endl;
        for (auto &one : *childrenPtr) {
          std::cout << "[" << one->Id() << "] " << one->Name() << std::endl;
        }
      }
      return;
    }
    error = trees[selectedTreeIndex]->GetError();
  } catch (...) {
    error = FamilyTree::Error::INVALID_TYPE;
  }
  if (FamilyTree::Error::INVALID_TYPE == error) {
    std::cout << selectedPersonPtr->Name() << " is not a parent." << std::endl;
  } else {
    std::cerr << "Unexpected Checkout Siblings Error" << std::endl;
  }
}
void UIController::checkoutExWives() {
  auto selectedPersonPtr = trees[selectedTreeIndex]->SelectPerson();
  auto error = trees[selectedTreeIndex]->GetError();
  try {
    auto exwivesPtr = trees[selectedTreeIndex]->GetExWives();
    if (exwivesPtr) {
      if (exwivesPtr->empty()) {
        std::cout << selectedPersonPtr->Name() << " is a good man." << std::endl;
      } else {
        std::cout << "Ex:" << std::endl;
        for (auto &one : *exwivesPtr) {
          std::cout << "[" << one->Id() << "] " << one->Name() << std::endl;
        }
      }
      return;
    }
    error = trees[selectedTreeIndex]->GetError();
  } catch (...) {
    error = FamilyTree::Error::INVALID_TYPE;
  }
  if (FamilyTree::Error::INVALID_TYPE == error) {
    std::cout << selectedPersonPtr->Name() << " is not a male." << std::endl;
  } else {
    std::cerr << "Unexpected Checkout Siblings Error" << std::endl;
  }
}

void UIController::displaySubTree() {
  auto selectedPersonPtr = trees[selectedTreeIndex]->SelectPerson();
  printPersonTree(selectedPersonPtr);
}

void gracefullyExit(int sig) {
#ifdef WIN32
  Terminal::getInstancePtr()->reset();
#endif
  std::cout << std::endl;
  switch (sig) {
    case SIGSEGV:
      std::cout << "[error] Something seems to have gone wrong. Check out the log for more information" << std::endl;
      exit(0);
      break;
    default:
      break;
  }
}
