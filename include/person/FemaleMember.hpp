#ifndef TREEWITHMALE_FEMALEMEMBER_HPP
#define TREEWITHMALE_FEMALEMEMBER_HPP

#include "BloodRelation.hpp"

class FemaleMember : public BloodRelation {
  public:
    FemaleMember(
        const Info &info, std::shared_ptr<Male> dad, std::shared_ptr<Wife> mom);

  private:
};

#endif //TREEWITHMALE_FEMALEMEMBER_HPP
