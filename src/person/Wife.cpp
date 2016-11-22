#include "person/Wife.hpp"
#include "person/Male.hpp"

using std::shared_ptr;

Wife::Wife(const Info &info, std::shared_ptr<Male> husband)
    : Parent(info, FEMALE), Person(info, FEMALE), husband_(husband),
      divorced_(false) {
    children_ = husband->children_;
}

bool Wife::IsMarried() const {
    return not divorced_;
}

bool Wife::DoDivorce() {
    if (not IsAlive() or not IsMarried()) {
        return false;
    }
    husband_ = shared_ptr<Male>();
    return (divorced_ = true);
}

std::shared_ptr<Parent> Wife::Couple() const {
    if (husband_.expired()) {
        return nullptr;
    }
    return shared_ptr<Parent>(husband_);
}

std::shared_ptr<Male> Wife::GetDaddy() {
    return shared_ptr<Male>(husband_);
}

std::shared_ptr<Wife> Wife::GetMommy() {
    return shared_from_this();
}
