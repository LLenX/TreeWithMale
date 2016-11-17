#include "person/BloodRelation.hpp"

BloodRelation::BloodRelation(
    const Info &info, PersonGender gender, std::shared_ptr<Male> dad,
    std::shared_ptr<Wife> mom)
    : Person(info, gender), father_(dad), mother_(mom),
      siblings_(dad->Children()) {}
