#include "utitlity/printTree.hpp"

int main() {
    FamilyTree tree;
    std::shared_ptr<const Person> someone =
    tree.CreateAncestor(Person::Info("ancestor", "ancestor"));
    tree.Marry(Person::Info("may have", "ancestor wife"));
    tree.GiveBirthTo(Person::Info("b1", "1"), Person::PersonGender::MALE);
    tree.GiveBirthTo(Person::Info("b2", "2"), Person::PersonGender::MALE);
    tree.GiveBirthTo(Person::Info("b3", "3"), Person::PersonGender::MALE);
    tree.SelectPerson(tree.GetChildren()->front()->Id());
    tree.Marry(Person::Info("w1", "w1"));
    tree.GiveBirthTo(Person::Info("s1:1", "1:1"), Person::PersonGender::MALE);
    tree.GiveBirthTo(Person::Info("s1:2", "1:2"), Person::PersonGender::MALE);
    tree.GiveBirthTo(Person::Info("s1:3", "1:3"), Person::PersonGender::MALE);
    std::cout << "1's sons: " << tree.SelectPerson() << std::endl;
    tree.SelectPerson("b2");
    tree.Marry(Person::Info("w2", "w2"));
    tree.GiveBirthTo(Person::Info("s2:1", "2:1"), Person::PersonGender::MALE);
    tree.GiveBirthTo(Person::Info("s2:2", "2:2"), Person::PersonGender::MALE);
    tree.GiveBirthTo(Person::Info("s2:3", "2:3"), Person::PersonGender::MALE);
    std::cout << "2's sons: " << tree.SelectPerson() << std::endl;
    tree.SelectPerson("b3");
    tree.Marry(Person::Info("w3", "w3"));
    tree.GiveBirthTo(Person::Info("s3:1", "3:1"), Person::PersonGender::MALE);
    tree.GiveBirthTo(Person::Info("s3:2", "3:2"), Person::PersonGender::MALE);
    tree.GiveBirthTo(Person::Info("s3:3", "3:3"), Person::PersonGender::MALE);
    printFamilyTree(tree);
}