#ifndef TREEWITHMALE_WIFE_HPP
#define TREEWITHMALE_WIFE_HPP

#include "Parent.hpp"

class Wife : public Parent, std::enable_shared_from_this<Wife> {
  public:
    Wife(const Info &info)
        : Parent(info, MALE), divorced_(false) {}

    /**
     * override from parent
     */
    virtual bool IsMarried() const override;

    /**
     * override from Parent
     */
    virtual bool Divorce() override;

  private:
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
