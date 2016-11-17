#ifndef TREEWITHMALE_WIFE_HPP
#define TREEWITHMALE_WIFE_HPP

#include "PersonWithChild.hpp"

class Wife: public PersonWithChild, std::enable_shared_from_this<Wife> {
  public:
    Wife(const Info& info);
  private:
};

#endif //TREEWITHMALE_WIFE_HPP
