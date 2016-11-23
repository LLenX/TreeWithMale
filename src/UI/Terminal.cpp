#ifndef WIN32
#include <memory>
#include "./UI/Terminal.hpp"
Terminal::Terminal(): oldSettings() {
            // stdin
  tcgetattr(STDIN_FILENO, &oldSettings);  // back up old terminal settings
}
void Terminal::noBufferInputMode() {
  termios newSettings;
  newSettings = oldSettings;  // new settings are based on old settings
                              // (ICANON: buffer mode: flush on capturing '\n' or eof)
  newSettings.c_lflag &= ~(ECHO | ICANON);  // turn off echo and buffer
                      // TCSANOW: TC Set Attribute NOW
  tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);  // apply new settings immediately
}
void Terminal::normalInputMode() {
  tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);  // apply old settings
}
std::shared_ptr<Terminal> Terminal::getInstancePtr() {
  static std::shared_ptr<Terminal> instancePtr(new Terminal());
  return instancePtr;
}
void Terminal::setFormat(unsigned code) {
  std::cout << "\e[" << code << "m" << std::flush;
}
void Terminal::resetFormat() {
  std::cout << "\e[0m" << std::flush;
}
void Terminal::setPromptFormat() {
  setFormat(91); // light red
}
void Terminal::setInputFormat() {
  setFormat(1);  // bold
  setFormat(92);  // light green
}
void Terminal::reset() {
  resetFormat(), normalInputMode();
}
Terminal::~Terminal() {
  reset();  // auto reset at exit
}

std::string prompt(const std::string &description, bool isPassword) {
  char c = 0;
  std::string result;
  auto TerminalInstancePtr = Terminal::getInstancePtr();
  if (description.length()) {
    if (description[0] == '[') TerminalInstancePtr->setPromptFormat();
    std::cout << description << std::flush;
  }
  TerminalInstancePtr->resetFormat();
  std::cout << ' ' << std::flush;
  TerminalInstancePtr->setInputFormat();
  TerminalInstancePtr->noBufferInputMode();
  while ( (c = static_cast<char>(std::cin.get())) != '\n') {
    if (c == 127) {
      if (result.length()) {
        TerminalInstancePtr->resetFormat();
        std::cout << "\b \b" << std::flush;
        TerminalInstancePtr->setInputFormat();
        result.pop_back();
      }
    } else if (c == 27) {
      
    } else {
      std::cout << (isPassword ? '*' : c) << std::flush;
      result.push_back(c);
    }
  }
  TerminalInstancePtr->reset();
  std::cout << std::endl; // for the input of '\n'
  return result;
}
#endif  // WIN32
