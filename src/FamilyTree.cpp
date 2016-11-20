#include "FamilyTree.hpp"

using std::shared_ptr;
using std::dynamic_pointer_cast;

FamilyTree::Error FamilyTree::GetError() const {
    return error_;
}

void FamilyTree::SetError(FamilyTree::Error error) {
    error_ = error;
}

shared_ptr<Person> FamilyTree::CreateAncestor(const Person::Info &info) {
    if (ancestor_) {
        SetError(Error::PERSON_CONFILCT);
        return nullptr;
    }
    person_record_[info.id] = ancestor_ = Person::CreateAncestor(info);
    return ancestor_;
}

std::shared_ptr<Person> FamilyTree::SelectPerson(const std::string &id) {
    auto search_result_iter = person_record_.find(id);
    if (search_result_iter == end(person_record_)) {
        SetError(Error::RESULT_NOT_FOUND);
    }
    return selected_person_ = search_result_iter->second;
}

bool FamilyTree::CheckPersonSelected() {
    if (not selected_person_) {
        SetError(Error::EMPTY_SELECTION);
        return false;
    }
    return true;
}

template<typename GuessType, typename CurrentType>
std::shared_ptr<GuessType>
FamilyTree::CheckAndCast(std::shared_ptr<CurrentType> person) {
    auto result_ptr = dynamic_pointer_cast<GuessType>(person);
    if (not result_ptr) {
        SetError(Error::INVALID_TYPE);
    }
    return result_ptr;
}

template<typename Result, typename Person>
std::shared_ptr<const Result> FamilyTree::GetAndCheckExist(
    std::shared_ptr<Person> person,
    shared_ptr<Result> (Person::*getter)() const) {
    std::shared_ptr<Result> result_ptr = ((*person).*getter)();
    if (not result_ptr) {
        SetError(Error::RESULT_NOT_FOUND);
    }
    return result_ptr;
}

template<typename Result, typename Person>
std::shared_ptr<Result>
FamilyTree::GetFamilyMember(shared_ptr<Result> (*getter)() const) {
    if (not CheckPersonSelected()) {
        return nullptr;
    }

    auto casted_person = CheckAndCast<Person>(selected_person_);
    if (not casted_person) {
        return nullptr;
    }

    return GetAndCheckExist(casted_person, getter);
}

std::shared_ptr<Wife> FamilyTree::GetMother() {
    return GetFamilyMember(&BloodRelation::Mother);
}

std::shared_ptr<Male> FamilyTree::GetFather() {
    return GetFamilyMember(&BloodRelation::Father);
}

std::shared_ptr<const Person::Vector<BloodRelation>> FamilyTree::GetSiblings() {
    return GetFamilyMember(&BloodRelation::Siblings);
}

std::shared_ptr<const Person::Vector<BloodRelation>> FamilyTree::GetChildren() {
    return GetFamilyMember(&Parent::Children);
}

bool FamilyTree::Divorce() {
    if (not CheckPersonSelected()) {
        return false;
    }
    auto parent = CheckAndCast<Parent>(selected_person_);
    return parent->Divorce();
}

bool FamilyTree::CheckConflictId(const std::string &id) {
    if (person_record_.find(id) != end(person_record_)) {
        SetError(Error::PERSON_CONFILCT);
        return true;
    }
    return false;
}

std::shared_ptr<Wife> FamilyTree::Marry(const Person::Info &info) {
    if (not CheckPersonSelected() or CheckConflictId(info.id)) {
        return nullptr;
    }
    auto male = CheckAndCast<Male>(selected_person_);
    if (not male) {
        return nullptr;
    }
    auto wife = male->Marry(info);
    if (not wife) {
        SetError(Error::WRONG_LOGIC);
    } else {
        person_record_[info.id] = wife;
    }
    return wife;
}

std::shared_ptr<BloodRelation>
FamilyTree::GiveBirthTo(const Person::Info &info, Person::PersonGender gender) {
    if (not CheckPersonSelected() or CheckConflictId(info.id)) {
        return nullptr;
    }
    auto parent = CheckAndCast<Parent>(selected_person_);
    if (not parent) {
        return nullptr;
    }
    auto baby = parent->GiveBirthTo(info, gender);
    if (not baby) {
        SetError(Error::WRONG_LOGIC);
    } else {
        person_record_[info.id] = baby;
    }
    return baby;
}
