#ifndef TREEWITHMALE_UI_HPP_
#define TREEWITHMALE_UI_HPP_

#include <string>
#include <functional>
#include <memory>
#include <stack>
#include "./UI/Menu.hpp"

class UIController;
class UI {
 public:
  friend class UIController;
  static std::shared_ptr<UI> getInstancePtr();
  void oneLoop();
  ~UI();
 private:
  std::shared_ptr<Menu> topMenuPtr;
  std::shared_ptr<Menu> curMenuPtr = nullptr;
  std::stack<std::shared_ptr<Menu>> menuStack;
  UI();
  std::string &&input(std::function<std::string(const std::string &)> isValid,
                      const std::string &message = "");
  static constexpr const char *defaultPrompt = ">>> ";
};

#endif  // TREEWITHMALE_UI_HPP_
