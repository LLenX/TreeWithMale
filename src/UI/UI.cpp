#include <memory>
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

void UI::loop() {
  curMenuPtr->show();
  input();
  curMenuPtr->has
}