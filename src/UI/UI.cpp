#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include "./UI/UI.hpp"
#include "./UI/Terminal.hpp"

UI::UI()
: topMenuPtr(new Menu("Main")) {
  curMenuPtr = topMenuPtr;
}

UI::~UI() {}

std::shared_ptr<UI> UI::getInstancePtr() {
  static std::shared_ptr<UI> instancePtr(new UI());
  return instancePtr;
}

static void trim(std::string &str, char trimmedChar = ' ') {
  if (0 == str.length()) return;
  std::string::size_type start = str.find_first_not_of(trimmedChar),
                         end = str.find_last_not_of(trimmedChar);
  if (start != std::string::npos) {
    str = str.substr(start, end - start + 1);
  } else {
    str.clear();
  }
}

std::string &&UI::input(std::function<std::string(const std::string &)> isValid,
                        const std::string &message) {
  std::string oneLine, failedPrompt,
              promptMsg = message.length() ? message : defaultPrompt;
  auto lengthBeforeTrimmed = oneLine.length();
  while (true) {
    if (std::cin.eof()) {
      return std::move(std::string("q"));
    }
#ifdef WIN32
    std::cout << promptMsg << std::flush;
    std::getline(std::cin, oneLine);
#else  // defined WIN32
    oneLine = prompt(promptMsg);
#endif  // not defined WIN32
    do {
      lengthBeforeTrimmed = oneLine.length();
      trim(oneLine, ' '), trim(oneLine, '\t');
    } while (oneLine.length() != lengthBeforeTrimmed);

    if (0 == oneLine.length()) continue;
    std::cout << std::endl;
    failedPrompt = isValid(oneLine);
    if (0 == failedPrompt.length()) break;
    std::cout << failedPrompt << std::endl << std::endl;
  }
  return std::move(oneLine);
}

void UI::oneLoop() {
  std::string operation = input([&](const std::string &op) -> std::string {
    if (curMenuPtr->hasOp(op)) {
      return "";
    } else {
      return "Omoshiroi Operation. Please try a valid one next time.";
    }
  });
  
  curMenuPtr->execOp(operation);
  if (curMenuPtr->opIsSubMenu(operation)) {
    curMenuPtr = curMenuPtr->getSubMenuPtrOf(operation);
    menuStack.push(curMenuPtr);
  }
  
}
