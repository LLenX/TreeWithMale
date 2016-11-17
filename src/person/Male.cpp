#include "person/Male.hpp"
#include "person/Wife.hpp"

Male::Male(
    const Person::Info &info, std::shared_ptr<Male> dad,
    std::shared_ptr<Wife> mom)
    : Person(info, MALE), Parent(info, MALE),
      BloodRelation(info, MALE, dad, mom) {}

bool Male::IsMarried() const {
    return wife_ != nullptr;
}

std::shared_ptr<Parent> Male::Couple() const {
    return wife_;
}

std::shared_ptr<Male> Male::GetDaddy() {
    return shared_from_this();
}

std::shared_ptr<Wife> Male::GetMommy() {
    return wife_;
}

bool Male::Divorce() {
    bool can_divorce = IsMarried();
    if (can_divorce) {
        ex_wives_->push_back(wife_);
        wife_.reset();
    }
    return can_divorce;
}

std::shared_ptr<const Person::Vector<Wife>>
Male::ExWives() const {
    return ex_wives_;
}

std::shared_ptr<Wife> Male::Marry(const Person::Info &info) {
    if (IsMarried()) {
        return nullptr;
    }
    return wife_ = std::shared_ptr<Wife>(new Wife(info, shared_from_this()));
}
