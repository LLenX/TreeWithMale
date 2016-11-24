#ifndef PRINTTREE_HPP
#define PRINTTREE_HPP
#include "FamilyTree.hpp"
#include <iostream>


/*
 * The struct for print the FamilyTree in format
 * person is the shared_ptr to one person in the FamilyTree
 * offset is the number of character between the begin of this
 *  line and the first character of this person.
 * child is the position of the child in the print map
 */
struct printStruct;

/*
 * overload the out stream operator << for the shared_ptr<const Person>
 * Print the name of a person if the length of name is less than the given length
 * The length whose default value is 8, can be set in function setOutLength.
 * @param out  the ostream
 * @param someone  a shared pointer to const Person, if the
 */
std::ostream &operator<<(std::ostream &out,
                        std::shared_ptr<const Person> someone);

static std::size_t PERSON_NAME_MAX_LENGTH = 9;

/*
 * Set the max length of a name
 * @param the new length is odd which must be greater than 3
 */
void setOutLength(std::size_t length);

/*
 * get the ancestor of a FamilyTree
 */
std::shared_ptr<const Person> getAncester(FamilyTree &tree);

void printFamilyTree(FamilyTree &tree);

void printPersonTree(std::shared_ptr<const Person> someone);

#endif /* PrintTree.hpp */
