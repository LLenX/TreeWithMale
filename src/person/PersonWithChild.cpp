#include "person/PersonWithChild.hpp"
#include "person/BloodRelation.hpp"

using std::make_shared;

PersonWithChild::PersonWithChild(
    const Person::Info &info, Person::PersonGender gender)
    : Person(info, gender), children_(make_shared<BloodRelation::Members>()) {}

std::shared_ptr<BloodRelation::ConstMembers> PersonWithChild::Children() const {
    return children_;
}
