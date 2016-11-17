#ifndef TREEWITHMALE_MALE_HPP
#define TREEWITHMALE_MALE_HPP

#include "Parent.hpp"
#include "BloodRelation.hpp"
#include "Wife.hpp"

class Male
    : public Parent,
      public BloodRelation,
      public std::enable_shared_from_this<Male> {
  public:
    Male(
        const Info &info, std::shared_ptr<Male> dad, std::shared_ptr<Wife> mom);

    /**
     * override from Parent
     */
    virtual bool IsMarried() const override;

    /**
     * override from Parent
     */
    virtual bool Divorce() override;

    /**
     * getter for ex-wives :)
     * @return the collection of the pointer point to ex-wives
     */
    std::shared_ptr<const std::vector<std::shared_ptr<Wife>>> ExWives() const;

    /**
     * marry to the woman with corresponding info, if this man is married
     * already, this operation failed with nullptr returned
     * @param info the information of the wife
     * @return the newly married wife
     */
    std::shared_ptr<Wife> Marry(const Info& info);

  private:
    virtual std::shared_ptr<Male> GetDaddy() override;

    virtual std::shared_ptr<Wife> GetMommy() override;

    /**
     * wife, null if doesn't have
     */
    std::shared_ptr<Wife> wife_;

    /**
     * ex_wives, you know this
     */
    std::shared_ptr<std::vector<std::shared_ptr<Wife>>> ex_wives_;
};

#endif //TREEWITHMALE_MALE_HPP
