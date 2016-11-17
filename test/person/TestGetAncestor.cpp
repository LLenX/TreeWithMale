#include "person/Person.hpp"
#include <iostream>

using std::cin;
using std::cout;

int main() {
    Person::Info info("12345", "Alvin");
    auto ptr = Person::CreateAncestor(info);
    cout << ptr->Gender() << ptr->Id() << ptr->Name() << ptr->IsAlive() << '\n';
}
