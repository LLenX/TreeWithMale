#ifndef TREEWITHMALE_PERSONWITHCHILD_HPP
#define TREEWITHMALE_PERSONWITHCHILD_HPP

#include "Person.hpp"
#include "BloodRelation.hpp"
#include <memory>

class PersonWithChild : virtual public Person {
  public:
    PersonWithChild(const Info& info, PersonGender gender);

    /**
     * give birth to a child and add him/her to children_
     * @param info the basic info of a person
     * @param gender the gender of the person
     */
    virtual void GiveBirthTo(const Info& info, PersonGender gender) = 0;

    /**
     * getter for the collection of children
     * @return the pointer to the const vector of the member
     */
    std::shared_ptr<BloodRelation::ConstMembers> Children() const;

  private:
    std::shared_ptr<BloodRelation::Members> children_;
};

#endif //TREEWITHMALE_PERSONWITHCHILD_HPP
