# TreeWithMale
Family tree for the project of course of data structure

# Compile

## Using cmake

Install clang before further operation.

```
cd path-to-this-folder/bin
cmake ..
make

```

## Directly

```
g++ ./src/main.cpp ./src/FamilyTree.cpp ./src/person/BloodRelation.cpp ./src/person/FemaleMember.cpp ./src/person/Male.cpp ./src/person/Parent.cpp ./src/person/Person.cpp ./src/person/Wife.cpp ./src/UI/Controller.cpp ./src/UI/Menu.cpp ./src/UI/MenuItem.cpp ./src/UI/Terminal.cpp ./src/UI/UI.cpp ./src/utility/PrintTree.cpp -o familytree -I ./include -std=c++14
```
