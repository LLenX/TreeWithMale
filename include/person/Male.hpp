#ifndef TREEWITHMALE_MALE_HPP
#define TREEWITHMALE_MALE_HPP

#include "PersonWithChild.hpp"
#include "BloodRelation.hpp"

class Male
    : public PersonWithChild,
      public BloodRelation,
      public std::enable_shared_from_this<Male> {
  public:
    Male(const Info& info);
  private:
};

#endif //TREEWITHMALE_MALE_HPP
