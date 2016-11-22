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
}

TEST_F(TreeTest, TestBirth) {
    family_tree.SelectPerson("Ancestor ID");
    ASSERT_TRUE(family_tree.SelectPerson());
    some_person = family_tree.GiveBirthTo(
        Person::Info("Cannot have baby", "before marry"), Person::MALE);
    EXPECT_FALSE(some_person);
    EXPECT_EQ(family_tree.GetError(), FamilyTree::Error::WRONG_LOGIC);
}

class TreeTest2 : public testing::Test {
  protected:
    void SetUp() {
        family_tree.CreateAncestor(Person::Info("Ancestor", "XMQ"));
        family_tree.Marry(Person::Info("FutureWife", "Unknown"));
    }

    std::shared_ptr<const Person> some_person;
    FamilyTree family_tree;
};

TEST_F(TreeTest2, TestMaleDivorce) {
    some_person = family_tree.SelectPerson("Ancestor");
    ASSERT_EQ(family_tree.GetCouple()->Id(), "FutureWife");
    family_tree.Divorce();
    EXPECT_FALSE(family_tree.GetCouple());
    EXPECT_EQ(family_tree.GetExWives()->size(), 1);
    EXPECT_EQ(family_tree.GetExWives()->front()->Id(), "FutureWife");
    ASSERT_TRUE(family_tree.Marry(Person::Info("Another Wife", "Whatever")));
}

TEST_F(TreeTest2, TestWifeDivorce) {
    some_person = family_tree.SelectPerson("FutureWife");
    EXPECT_EQ(some_person->Id(), "FutureWife");

    family_tree.Divorce();
    EXPECT_FALSE(family_tree.GetCouple());
    EXPECT_EQ(family_tree.GetError(), FamilyTree::Error::RESULT_NOT_FOUND);
}

TEST_F(TreeTest2, TestBirth) {
    some_person = family_tree.SelectPerson("Ancestor");
    EXPECT_TRUE(family_tree.GetChildren()->empty());

    family_tree.GiveBirthTo(Person::Info("Child1", "child1"), Person::MALE);
    EXPECT_EQ(family_tree.GetChildren()->front()->Id(), "Child1");
    some_person = family_tree.SelectPerson("Child1");
    ASSERT_TRUE(some_person);
    EXPECT_EQ(some_person->Id(), "Child1");

    family_tree.SelectPerson("FutureWife");
    family_tree.GiveBirthTo(Person::Info("Child2", "child2"), Person::FEMALE);
    EXPECT_EQ(family_tree.GetChildren()->size(), 2);
    some_person = family_tree.SelectPerson("Child2");
    EXPECT_EQ(some_person->Id(), "Child2");

    family_tree.SelectPerson("Child2");
    EXPECT_EQ(family_tree.GetSiblings()->front()->Id(), "Child1");
    EXPECT_EQ(family_tree.GetSiblings()->size(), 1);
    EXPECT_EQ(family_tree.GetFather()->Id(), "Ancestor");
    EXPECT_EQ(family_tree.GetMother()->Id(), "FutureWife");

    family_tree.SelectPerson("Child1");
    EXPECT_EQ(family_tree.GetSiblings()->front()->Id(), "Child2");
    EXPECT_EQ(family_tree.GetSiblings()->size(), 1);
    EXPECT_EQ(family_tree.GetFather()->Id(), "Ancestor");
    EXPECT_EQ(family_tree.GetMother()->Id(), "FutureWife");

    family_tree.SelectPerson("Ancestor");
    family_tree.Divorce();
    EXPECT_FALSE(family_tree.GiveBirthTo(
        Person::Info("Cannot give birth to a baby", "After divorce"),
        Person::MALE));
    EXPECT_EQ(family_tree.GetError(), FamilyTree::Error::WRONG_LOGIC);

    family_tree.SelectPerson("FutureWife");
    EXPECT_FALSE(family_tree.GiveBirthTo(
        Person::Info("Cannot give birth to a baby", "After divorce"),
        Person::MALE));
    EXPECT_EQ(family_tree.GetError(), FamilyTree::Error::WRONG_LOGIC);

}

TEST_F(TreeTest2, TestError) {
    some_person = family_tree.SelectPerson("FutureWife");
    ASSERT_TRUE(some_person);
    some_person = family_tree.GetMother();
    EXPECT_FALSE(some_person);
    EXPECT_EQ(family_tree.GetError(), FamilyTree::Error::INVALID_TYPE);
    some_person = family_tree.GetFather();
    EXPECT_FALSE(some_person);
    EXPECT_EQ(family_tree.GetError(), FamilyTree::Error::INVALID_TYPE);

    EXPECT_FALSE(family_tree.GetSiblings());
    EXPECT_EQ(family_tree.GetError(), FamilyTree::Error::INVALID_TYPE);

    family_tree.GiveBirthTo(Person::Info("Child2", "child2"), Person::FEMALE);
    family_tree.SelectPerson("Child2");
    EXPECT_FALSE(family_tree.GetChildren());
}