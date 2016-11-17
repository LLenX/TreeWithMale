#include "person/BloodRelation.hpp"
#include "person/Male.hpp"

BloodRelation::BloodRelation(
    const Info &info, PersonGender gender, std::shared_ptr<Male> dad,
    std::shared_ptr<Wife> mom)
    : Person(info, gender), father_(dad), mother_(mom) {
    if (dad) {
        siblings_ = dad->Children();
    }
}

std::shared_ptr<Male> BloodRelation::Father() const {
    return std::shared_ptr<Male>(father_);
}

std::shared_ptr<Wife> BloodRelation::Mother() const {
    return std::shared_ptr<Wife>(mother_);
}

std::shared_ptr<Person::Vector<BloodRelation>>
BloodRelation::Siblings() const {
    using Siblings = Person::Vector<BloodRelation>;

    std::shared_ptr<const Siblings> siblings_before_filter(
        siblings_);
    std::shared_ptr<Siblings> result_siblings = std::make_shared<Siblings>(
        *siblings_before_filter);

    // filter out him/herself
    for (auto iter = result_siblings->begin();
         iter != result_siblings->end(); ++iter) {
        if (**iter == *this) { // *iter is pointer, **iter is the person
            result_siblings->erase(iter);
            break;
        }
    }
    return result_siblings;
}
