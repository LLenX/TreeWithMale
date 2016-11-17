#include "person/FemaleMember.hpp"

FemaleMember::FemaleMember(
    const Person::Info &info, std::shared_ptr<Male> dad,
    std::shared_ptr<Wife> mom)
    : BloodRelation(info, FEMALE, dad, mom), Person(info, FEMALE) {}

