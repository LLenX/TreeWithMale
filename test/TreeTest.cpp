#include "FamilyTree.hpp"
#include "gtest/gtest.h"

class TreeTest : public testing::Test {
  protected:
    void SetUp() {
        some_person = family_tree.CreateAncestor(
            Person::Info("Ancestor ID", "xmq"));
    }

    std::shared_ptr<const Person> some_person;
    FamilyTree family_tree;
};

TEST_F(TreeTest, TestAncestor) {
    EXPECT_EQ(some_person->Name(), "xmq");
    EXPECT_EQ(some_person->Id(), "Ancestor ID");

    some_person = family_tree.CreateAncestor(
        Person::Info("Another Ancestor", "Whatever"));
    EXPECT_FALSE(some_person);
    EXPECT_EQ(family_tree.GetError(), FamilyTree::Error::PERSON_CONFILCT);

}

TEST_F(TreeTest, TestSelect) {
    some_person = family_tree.SelectPerson("Some Other Person");
    EXPECT_FALSE(some_person);
    EXPECT_EQ(family_tree.GetError(), FamilyTree::Error::RESULT_NOT_FOUND);

    some_person = family_tree.SelectPerson("Ancestor ID");
    ASSERT_TRUE(some_person);
    EXPECT_EQ(some_person->Name(), "xmq");
    EXPECT_EQ(some_person->Id(), "Ancestor ID");
}

TEST_F(TreeTest, TestMarry) {
    some_person = family_tree.Marry(Person::Info("future wife id", "Unknown"));
    ASSERT_TRUE(some_person);
    EXPECT_EQ(some_person->Id(), "future wife id");

    some_person = family_tree.Marry(
        Person::Info("second wife before divorce", "Unknown"));
    EXPECT_FALSE(some_person);
    EXPECT_EQ(family_tree.GetError(), FamilyTree::Error::WRONG_LOGIC);

    some_person = family_tree.GetCouple();
    EXPECT_EQ(some_person->Id(), "future wife id");

    some_person = family_tree.SelectPerson("future wife id");
    EXPECT_EQ(some_person->Id(), "future wife id");
}