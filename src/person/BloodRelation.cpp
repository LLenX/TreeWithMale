#include "person/BloodRelation.hpp"
#include "person/Male.hpp"

BloodRelation::BloodRelation(
    const Info &info, PersonGender gender, std::shared_ptr<Male> dad,
    std::shared_ptr<Wife> mom)
    : Person(info, gender), father_(dad), mother_(mom),
      siblings_(dad->Children()) {}

std::shared_ptr<Male> BloodRelation::Father() const {
    return std::shared_ptr<Male>(father_);
}

std::shared_ptr<Wife> BloodRelation::Mother() const {
    return std::shared_ptr<Wife>(mother_);
}
