#ifndef WIN32
#ifndef TREEWITHMALE_UI_TERMINAL_HPP_
#define TREEWITHMALE_UI_TERMINAL_HPP_
#include <termios.h>
#include <unistd.h>
#include <memory>
#include <iostream>
class Terminal {
 public:
  Terminal();
  void noBufferInputMode();
  void normalInputMode() ;
  void setFormat(unsigned code);
  void resetFormat();
  void setPromptFormat();
  void setInputFormat();
  void reset();
  ~Terminal();
  static std::shared_ptr<Terminal> getInstancePtr();
 private:
  termios oldSettings;
};
std::string prompt(const std::string &description, bool isPassword = false);
#endif  // TREEWITHMALE_UI_TERMINAL_HPP_
#endif  // WIN32
