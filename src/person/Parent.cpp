#include "person/Parent.hpp"
#include "person/BloodRelation.hpp"
#include "person/Male.hpp"
#include "person/FemaleMember.hpp"

using std::make_shared;
using std::shared_ptr;

Parent::Parent(
    const Person::Info &info, Person::PersonGender gender)
    : Person(info, gender), children_(make_shared<BloodRelation::Members>()) {}

shared_ptr<BloodRelation::ConstMembers> Parent::Children() const {
    return children_;
}

shared_ptr<BloodRelation>
Parent::GiveBirthTo(const Person::Info &info, Person::PersonGender gender) {
    auto dad = GetDaddy();
    auto mom = GetMommy();
    shared_ptr<BloodRelation> child =
        gender == MALE ? make_shared<Male>(info, dad, mom) : make_shared<
            FemaleMember>(info, dad, mom);
    children_->push_back(child);
    return child;
}

