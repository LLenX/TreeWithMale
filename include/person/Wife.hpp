#ifndef TREEWITHMALE_WIFE_HPP
#define TREEWITHMALE_WIFE_HPP

#include "Parent.hpp"

class Wife : public Parent, public std::enable_shared_from_this<Wife> {
    /**
     * only Male can construct the wife (by marry)
     */
    friend class Male;

  public:
    /**
     * override from parent
     */
    virtual bool IsMarried() const override;

    /**
     * override from Parent
     */
    virtual bool Divorce() override;

    /**
     * override from Parent
     */
    virtual std::shared_ptr<Parent> Couple() const override;

  private:
    Wife(const Info &info);

    virtual std::shared_ptr<Male> GetDaddy() override;

    virtual std::shared_ptr<Wife> GetMommy() override;

    /**
     * whether the wife is divorced
     */
    bool divorced_;

    /**
     * husband of the wife, doesn't change after divorce.
     */
    std::weak_ptr<Male> husband_;
};

#endif //TREEWITHMALE_WIFE_HPP
