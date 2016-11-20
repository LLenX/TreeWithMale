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
    BloodRelation(
        const Info &info, PersonGender gender, std::shared_ptr<Male> dad,
        std::shared_ptr<Wife> mom);

    /**
     * getter for father
     * @return the pointer pointing to father
     */
    std::shared_ptr<Male> Father() const;

    /**
     * getter for wife
     * @return the pointer pointing to mother
     */
    std::shared_ptr<Wife> Mother() const;

    /**
     * getter for the siblings of the family member
     * @return the siblings not containing the person him/herself
     */
    std::shared_ptr<Person::Vector<BloodRelation>> Siblings() const;

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
    std::weak_ptr<const Person::Vector<BloodRelation>> siblings_;
};

#endif //TREEWITHMALE_BLOODRELATION_HPP
