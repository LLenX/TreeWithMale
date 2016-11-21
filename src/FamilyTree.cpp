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
    selected_person_ = person_record_[info.id] = ancestor_ = Person::CreateAncestor(
        info);
    return ancestor_;
}

std::shared_ptr<Person> FamilyTree::SelectPerson(const std::string &id) {
    auto search_result_iter = person_record_.find(id);
    if (search_result_iter == end(person_record_)) {
        SetError(Error::RESULT_NOT_FOUND);
        return nullptr;
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

std::shared_ptr<const Wife> FamilyTree::GetMother() {
    return GetFamilyMember(&BloodRelation::Mother);
}

std::shared_ptr<const Male> FamilyTree::GetFather() {
    return GetFamilyMember(&BloodRelation::Father);
}

std::shared_ptr<const Parent> FamilyTree::GetCouple() {
    return GetFamilyMember(&Parent::Couple);
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

bool FamilyTree::Die() {
    if (not CheckPersonSelected()) {
        return false;
    }
    bool success = selected_person_->Die();
    if (not success) {
        SetError(Error::WRONG_LOGIC);
    }
    return success;
}
