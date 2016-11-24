#ifndef PRINTTREE_HPP
#define PRINTTREE_HPP
#include "FamilyTree.hpp"


/*
 * The struct for print the FamilyTree in format
 * person is the shared_ptr to one person in the FamilyTree
 * offset is the number of character between the begin of this
 *  line and the first character of this person.
 * child is the position of the child in the print map
 */
struct printStruct {
    using Person_p = std::shared_ptr<const Person>;
    Person_p person;
    int offset;
    int child[100];
    printStruct(Person_p person_, int offset_ = 0): person(person_), offset(offset_), child(0) {}
};

/*
 * overload the out stream operator << for the shared_ptr<const Person>
 * Print the name of a person if the length of name is less than the given length
 * The length whose default value is 8, can be set in function setOutLength.
 * @param out  the ostream
 * @param someone  a shared pointer to const Person, if the
 */
std::ostream &operator<<(std::ostream &out,
                        std::shared_ptr<const Person> someone);

std::size_t PERSON_NAME_MAX_LENGTH = 9;

/*
 * Set the max length of a name
 * @param the new length is odd which must be greater than 3
 */
void setOutLength(std::size_t length);

/*
 * get the ancestor of a FamilyTree
 */
std::shared_ptr<const Person> getAncester(FamilyTree &tree);



#endif /* PrintTree.hpp */
