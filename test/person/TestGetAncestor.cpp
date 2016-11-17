#include "person/Person.hpp"

int main() {
    Person::Info info("12345", "Alvin");
    auto ptr = Person::CreateAncestor(info);
}