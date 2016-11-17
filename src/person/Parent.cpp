#include "person/Parent.hpp"
#include "person/Male.hpp"
#include "person/FemaleMember.hpp"

using std::make_shared;
using std::shared_ptr;

Parent::Parent(
    const Person::Info &info, Person::PersonGender gender)
    : Person(info, gender),
      children_(make_shared<Person::Vector<BloodRelation>>()) {}

shared_ptr<const Person::Vector<BloodRelation>> Parent::Children() const {
    return children_;
}

shared_ptr<BloodRelation>
Parent::GiveBirthTo(const Person::Info &info, Person::PersonGender gender) {
    auto dad = GetDaddy();
    auto mom = GetMommy();
    shared_ptr<BloodRelation> child = gender == MALE ? shared_ptr<Male>(
        new Male(info, dad, mom)) : shared_ptr<FemaleMember>(
        new FemaleMember(info, dad, mom));
    children_->push_back(child);
    return child;
}

