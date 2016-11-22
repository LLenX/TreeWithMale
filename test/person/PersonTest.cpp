#include "gtest/gtest.h"
#include "person/Male.hpp"
#include "person/Person.hpp"

class PersonTest : public testing::Test {
  protected:
    virtual void SetUp() {
        std::string name = "Name", id = "ID";
        ancestor = Person::CreateAncestor(
            Person::Info(id, name));
        EXPECT_EQ(ancestor->Name(), name);
        EXPECT_EQ(ancestor->Id(), id);
    }

    std::shared_ptr<Person> ancestor;
};

TEST_F(PersonTest, TestDie) {
    EXPECT_TRUE(ancestor->IsAlive());
    ancestor->Die();
    EXPECT_FALSE(ancestor->IsAlive());
}

TEST_F(PersonTest, TestGender) {
    EXPECT_EQ(ancestor->Gender(), Person::MALE);
}

class MaleTest : public testing::Test {
  protected:
    virtual void SetUp() {
        father = std::dynamic_pointer_cast<Male>(
            Person::CreateAncestor(
                Person::Info("father id", "father name")));
        ASSERT_TRUE(father->IsAlive());
    }

    std::shared_ptr<Male> father;
};

TEST_F(MaleTest, TestMarry) {
    EXPECT_FALSE(father->IsMarried());
    std::shared_ptr<Wife> kate = father->Marry(Person::Info("Wife Id", "Kate"));

    ASSERT_EQ(kate->Gender(), Person::FEMALE);
    EXPECT_TRUE(father->IsMarried());
    EXPECT_TRUE(kate->IsMarried());

    std::shared_ptr<Wife> anonymous = father->Marry(
        Person::Info("Another Wife", "Anonymous"));

    EXPECT_EQ(anonymous, nullptr);

    father->Divorce();

    EXPECT_FALSE(father->IsMarried());
    EXPECT_FALSE(kate->IsMarried());
    EXPECT_EQ(*father->ExWives()->front(), *kate);
}

TEST_F(MaleTest, TestBirth) {
    Person::Info child_info = {"Child Id", "John"};
    std::shared_ptr<BloodRelation> john = father->GiveBirthTo(
        child_info, Person::MALE);
    EXPECT_EQ(john, nullptr); // father don't even get married!
    std::shared_ptr<Wife> kate = father->Marry(Person::Info("Wife Id", "Kate"));
    john = father->GiveBirthTo(child_info, Person::MALE); // now should be ok
    ASSERT_EQ(john->Gender(), Person::MALE);
    EXPECT_EQ(*father->Children()->front(), *john);
    EXPECT_EQ(*kate->Children()->front(), *john);
}

TEST_F(MaleTest, TestSibling) {
    father->Marry(Person::Info("Wife Id", "Kate"));
    auto john = father->GiveBirthTo(
        Person::Info("Child Id", "John"),
        Person::MALE), catherine = father->GiveBirthTo(
        Person::Info("Child Id2", "Catherine"), Person::FEMALE);
    std::shared_ptr<Person::Vector<
        BloodRelation>> johns_siblings = john->Siblings(), catherines_sibilings = catherine->Siblings();

    EXPECT_EQ(johns_siblings->size(), 1);
    EXPECT_EQ(catherines_sibilings->size(), 1);
    EXPECT_EQ(*johns_siblings->front(), *catherine);
    EXPECT_EQ(*catherines_sibilings->front(), *john);
}

class WifeTest : public testing::Test {
  protected:
    virtual void SetUp() {
        chen_xu = std::dynamic_pointer_cast<Male>(
            Person::CreateAncestor(Person::Info("15331111", "ChenXu")));
        chen_yuxiang = chen_xu->Marry(Person::Info("15332222", "ChenZiXiang"));
    }

    std::shared_ptr<Male> chen_xu;
    std::shared_ptr<Wife> chen_yuxiang;
};

TEST_F(WifeTest, TestDivorce) {
    ASSERT_TRUE(chen_yuxiang->IsMarried());
    ASSERT_TRUE(chen_xu->IsMarried());

    chen_yuxiang->Divorce();

    EXPECT_FALSE(chen_yuxiang->IsMarried());
    EXPECT_FALSE(chen_xu->IsMarried());
    EXPECT_EQ(*chen_xu->ExWives()->front(), *chen_yuxiang);
    EXPECT_FALSE(chen_xu->Divorce());
}

TEST_F(WifeTest, TestBirth) {
    auto john = chen_yuxiang->GiveBirthTo(
        Person::Info("Child Id", "John"),
        Person::MALE), catherine = chen_yuxiang->GiveBirthTo(
        Person::Info("Child Id2", "Catherine"), Person::FEMALE);
    std::shared_ptr<Person::Vector<
        BloodRelation>> johns_siblings = john->Siblings(), catherines_sibilings = catherine->Siblings();

    EXPECT_EQ(johns_siblings->size(), 1);
    EXPECT_EQ(catherines_sibilings->size(), 1);
    EXPECT_EQ(*johns_siblings->front(), *catherine);
    EXPECT_EQ(*catherines_sibilings->front(), *john);
}
