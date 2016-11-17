#include "person/Person.hpp"
#include "person/Male.hpp"

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

bool Person::operator==(const Person &rhs) const {
    return rhs.Id() == Id();
}

bool Person::operator!=(const Person &rhs) const {
    return not(*this == rhs);
}

std::shared_ptr<Person> Person::CreateAncestor(const Person::Info &info) {
    return std::shared_ptr<Person>(new Male(info, nullptr, nullptr));
}

