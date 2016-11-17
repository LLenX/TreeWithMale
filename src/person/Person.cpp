#include "person/Person.hpp"

Person::Person(const Info &info, PersonGender gender)
    : info_(info), is_alive_(true), gender_(gender) {}

bool Person::IsAlive() const {
    return is_alive_;
}

void Person::Die() {
    is_alive_ = false;
}

const std::string &Person::Id() const {
    return info_.id;
}

Person::PersonGender Person::Gender() const {
    return gender_;
}

Person::~Person() {}

