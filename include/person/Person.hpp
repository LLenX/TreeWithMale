#ifndef TREEWITHMALE_PERSON_HPP_
#define TREEWITHMALE_PERSON_HPP_

#include <string>
#include <set>
#include <memory>
#include <vector>
#include <stdint.h>

/**
 * base class for male and female
 */
class Person {
  public:
    /**
     * the basic infomation of a person
     */
    struct Info {
        Info(const std::string &id, const std::string &name)
            : id(id), name(name) {}

        /**
         * the identity of a person, should be unique, if not, the behavior of
         * other method is undefined.
         * A id of a person should not be modified anyway
         */
        const std::string id;

        /**
         * the name of the person which could be duplicated
         */
        std::string name;
    };

    enum PersonGender {
        MALE, FEMALE
    };

    template<typename PersonType>
    using Vector<PersonType> = std::vector<std::shared_ptr<PersonType>>;

    /**
     * constructor initialize basic information of a person
     * @param info the name of the person
     */
    Person(const Info &info, PersonGender gender);

    /**
     * you cannot copy a person!
     */
    Person(const Person &) = delete;

    /**
     * you cannot copy a person!
     */
    Person &operator=(const Person &) = delete;

    /**
     * default move ctor
     */
    Person(Person &&) = default;

    /**
     * default move assignment
     */
    Person &operator=(Person &&) = default;

    /**
     * virtual dtor, do nothing
     */
    virtual ~Person();

    /**
     * whether the person is male
     * @return true if the person is male, false if not
     */
    virtual bool IsMale() const = 0;

    /**
     * whether the person is alive
     * @return true if the person is alive, false if not
     */
    bool IsAlive() const;

    /**
     * kill the person
     */
    void Die();

    /**
     * gender of a person, should be compare to Person::MALE or Person::FEMALE
     */
    PersonGender Gender() const;

    /**
     * getter for the id of the person
     * @return the reference to the id of a person
     */
    const std::string &Id() const;

  private:
    /**
     * basic infomation of the person
     */
    Info info_;

    /**
     * whether the person is alive
     */
    bool is_alive_;

    /**
     * the gender of the person
     */
    const PersonGender gender_;
};


#endif //TREEWITHMALE_PERSON_HPP_
