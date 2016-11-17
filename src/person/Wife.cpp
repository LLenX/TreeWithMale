#include "person/Wife.hpp"

using std::shared_ptr;

Wife::Wife(const Person::Info &info)
    : Parent(info, FEMALE) {}

bool Wife::IsMarried() const {
    return not divorced_;
}

bool Wife::Divorce() {
    bool can_divorce = not divorced_;
    divorced_ = true;
    return can_divorce;
}

std::shared_ptr<Male> Wife::GetDaddy() {
    return shared_ptr<Male>(husband_);
}

std::shared_ptr<Wife> Wife::GetMommy() {
    return shared_from_this();
}
