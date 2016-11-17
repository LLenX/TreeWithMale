#ifndef TREEWITHMALE_BLOODRELATION_HPP
#define TREEWITHMALE_BLOODRELATION_HPP

#include "Person.hpp"
#include <memory>

/**
 * class Wife: public PersonWithChild
 */
class Wife;

/**
 * class Male: public Blood Relation, public PersonWithChild
 */
class Male;

class BloodRelation : virtual public Person {
  public:
    using Self = BloodRelation;

    /**
     * too lazy to write a single class contains multiple family members
     * (with iterators and blabla)
     */
    using Members = std::vector<std::shared_ptr<Self>>;

    using ConstMembers = const std::vector<std::shared_ptr<Self>>;

    BloodRelation(
        const Info &info, PersonGender gender, std::shared_ptr<Male> dad,
        std::shared_ptr<Wife> mom);

  private:
    /**
     * weak reference to father, prevent circular reference
     */
    std::weak_ptr<Male> father_;

    /**
     * weak reference to mother, prevent circular reference
     */
    std::weak_ptr<Wife> mother_;

    /**
     * weak reference to the children of the person's father/mother
     * which means siblings_ contains the person him/herself
     */
    std::weak_ptr<ConstMembers> siblings_;
};

#endif //TREEWITHMALE_BLOODRELATION_HPP
