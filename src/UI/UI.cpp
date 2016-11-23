#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include "UI/UI.hpp"
UI::UI() {
  curMenuPtr = topMenuPtr;
}

UI::~UI() {

}

std::shared_ptr<UI> UI::getInstancePtr() const {
  static std::shared_ptr<UI> instancePtr = UI();
  return instancePtr;
}

static void trim(std::string &str) {
  if (0 == str.length()) return;
  std::string::size_type start = str.find_first_not_of(' '),
                         end = str.find_last_not_of(' ');
  if (start != std::string::npos) {
    str = str.substr(start, end - start + 1);
  } else {
    str.clear();
  }
}

std::string &&UI::inputOp(std::function<bool (*)(const std::string &)> isValid) {
  std::string oneLine;
  std::stringstream ss;
  while (true) {
    std::cout << prompt << std::flush;
    std::getline(std::cin, oneLine);
    trim(oneLine);
    if (0 == oneLine.length()) {
      continue;
    }
    if (isValid(oneLine)) {
      break;
    }
    std::cout << "\nUnrecognized Operation. Please try again.\n" << std::endl;
  }
  return std::move(oneLine);
}

void UI::loop() {
  curMenuPtr->show();
  std::string op = inputOp([&](const std::string &op) -> bool {
    return curMenuPtr->hasOp(key);
  });
  curMenuPtr->execOp(op);
}