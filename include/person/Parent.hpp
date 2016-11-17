#ifndef TREEWITHMALE_PERSONWITHCHILD_HPP
#define TREEWITHMALE_PERSONWITHCHILD_HPP

#include "Person.hpp"
#include "BloodRelation.hpp"
#include <memory>

class Parent : virtual public Person {
  public:
    Parent(const Info &info, PersonGender gender);

    /**
     * give birth to a child and add him/her to children_
     * @param info the basic info of a person
     * @param gender the gender of the person
     * @return the child born
     */
    std::shared_ptr<BloodRelation>
    GiveBirthTo(const Info &info, PersonGender gender);

    /**
     * getter for the collection of children
     * @return the pointer to the const vector of the member
     */
    std::shared_ptr<BloodRelation::ConstMembers> Children() const;

    /**
     * divorce current couple if has any
     * (if he/she has a couple, divorce may never fail)
     * @return true if the parent has a couple to divorce, false if not
     */
    virtual bool Divorce() = 0;

    /**
     * determine whether the parent(PARENT is just a NAME) is married
     * @return true if married, false if not
     */
    virtual bool IsMarried() const = 0;

    /**
     * getter for the couple of the parent, null if doesn't have any
     * @return the pointer pointing to the couple
     */
    virtual std::shared_ptr<Parent> Couple() const = 0;

  protected:
    /**
     * get the dad of the child
     * @return the pointer pointing to the dad of the child
     */
    virtual std::shared_ptr<Male> GetDaddy() = 0;

    /**
     * get the mom of the child
     * @return the pointer pointing to the mom of the child
     */
    virtual std::shared_ptr<Wife> GetMommy() = 0;

    /**
     * children of the parent
     */
    std::shared_ptr<BloodRelation::Members> children_;
};

#endif //TREEWITHMALE_PERSONWITHCHILD_HPP
