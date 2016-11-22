#ifndef TREEWITHMALE_MALE_HPP
#define TREEWITHMALE_MALE_HPP

#include "Parent.hpp"
#include "BloodRelation.hpp"
#include "Wife.hpp"

class Male
    : public Parent,
      public BloodRelation,
      public std::enable_shared_from_this<Male> {
    /**
     * only parent and GetAncestor can initialize a male
     */
    friend class Parent;

    friend std::shared_ptr<Person> Person::CreateAncestor(const Info &info);

    /**
     * enable wife to access the children when initialize
     */
    friend class Wife;

  public:
    /**
     * override from Parent
     */
    virtual bool IsMarried() const override;

    /**
     * override from Parent
     */
    virtual std::shared_ptr<Parent> Couple() const override;

    /**
     * getter for ex-wives :)
     * @return the collection of the pointer point to ex-wives
     */
    std::shared_ptr<const Person::Vector<Wife>> ExWives() const;

    /**
     * marry to the woman with corresponding info, if this man is married
     * already, this operation failed with nullptr returned
     * @param info the information of the wife
     * @return the newly married wife
     */
    std::shared_ptr<Wife> Marry(const Info &info);

  private:
    Male(
        const Info &info, std::shared_ptr<Male> dad, std::shared_ptr<Wife> mom);

    /**
     * override from Parent
     */
    virtual bool DoDivorce() override;

    virtual std::shared_ptr<Male> GetDaddy() override;

    virtual std::shared_ptr<Wife> GetMommy() override;

    /**
     * wife, null if doesn't have
     */
    std::shared_ptr<Wife> wife_;

    /**
     * ex_wives, you know this
     */
    std::shared_ptr<Person::Vector<Wife>> ex_wives_;
};

#endif //TREEWITHMALE_MALE_HPP
