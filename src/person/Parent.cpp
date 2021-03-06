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
    if (not IsAlive() or not IsMarried()) {
        return nullptr;
    }
    auto dad = GetDaddy();
    auto mom = GetMommy();
    shared_ptr<BloodRelation> child =
        gender == MALE ? shared_ptr<BloodRelation>(
            new Male(info, dad, mom)) : shared_ptr<BloodRelation>(
            new FemaleMember(info, dad, mom));
    children_->push_back(child);
    return child;
}

bool Parent::Divorce() {
    auto couple = Couple();
    if (not Couple()) {
        return false;
    }
    return couple->DoDivorce() and DoDivorce();
}

