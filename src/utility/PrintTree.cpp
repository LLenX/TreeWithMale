#include "utility/PrintTree.hpp"
#include <vector>
#include <queue>

struct printStruct {
    using Person_p = std::shared_ptr<const Person>;
    Person_p person;
    int offset, parent, child_num;
    printStruct(Person_p person_, int parent_ = -1)
        : person(person_), parent(parent_), offset(0) {
            child_num = std::dynamic_pointer_cast<const Parent>(person)->Children()->size();
        }
};
using PrintMap = std::shared_ptr<std::vector<std::vector<printStruct>>>;

std::ostream &operator<<(std::ostream &out,
                        std::shared_ptr<const Person> someone) {
    std::string name = someone->Name();
    if (name.length() > PERSON_NAME_MAX_LENGTH) {
        name = name.substr(0, PERSON_NAME_MAX_LENGTH - 3) + "...";
    } else if (name.length() < PERSON_NAME_MAX_LENGTH) {
        int prev_len = (PERSON_NAME_MAX_LENGTH - name.length()) / 2,
            post_len = (PERSON_NAME_MAX_LENGTH + 1 - name.length()) / 2;
        name = std::string(prev_len, ' ') + name + std::string(post_len, ' ');
    }
    std::cout << name;
    return out;
}

std::shared_ptr<const Person> getAncestor(FamilyTree &tree) {
    auto current_person_save = tree.SelectPerson();
    if (current_person_save == nullptr &&
        tree.GetError() == FamilyTree::Error::RESULT_NOT_FOUND)
        return nullptr;
    auto current_person = current_person_save;
    for ( ; ;) {
        try {
            current_person = tree.GetFather();
            current_person = tree.SelectPerson(current_person->Id());
        } catch (const std::bad_weak_ptr& err) {
            tree.SelectPerson(current_person_save->Id());
            return current_person;
        } 
    }
}

void VisitByLevel(std::queue<std::shared_ptr<const Person>>& queue,
                  PrintMap treeMap) {
    if (queue.empty()) return;
    std::queue<std::shared_ptr<const Person>> next_level;
    treeMap->push_back(std::vector<printStruct>());
    int parent_pos = 0;
    while (!queue.empty()) {
        auto front = queue.front();
        for (auto item : *std::dynamic_pointer_cast<const Parent>(front)->Children()) {
            auto cur_person = std::dynamic_pointer_cast<const Person>(item);
            next_level.push(cur_person);
            treeMap->back().push_back(printStruct(cur_person, parent_pos));
        }
        ++parent_pos;
        queue.pop();
    }
    if (!next_level.empty()) VisitByLevel(next_level, treeMap);
    else treeMap->pop_back();
    return;
}

void initOffset(PrintMap treeMap) {
    int prevOffset = -PERSON_NAME_MAX_LENGTH;
    auto& lastLevel = treeMap->back();
    for (auto& obj: lastLevel) {
        prevOffset = obj.offset = prevOffset + PERSON_NAME_MAX_LENGTH;
    }
    if (treeMap->size() == 1) return;
    for (auto levelIter = treeMap->end() - 1; levelIter != treeMap->begin(); --levelIter) {
        for (auto objp = levelIter->begin(); objp != levelIter->end(); ++objp) {
            if (objp->child_num) objp->offset /= objp->child_num;
            else if (objp != levelIter->begin()) objp->offset = (objp - 1)->offset + PERSON_NAME_MAX_LENGTH;
            (levelIter - 1)->at(objp->parent).offset += objp->offset;
        }
    }
    auto& firstLevel = treeMap->front();
    for (auto& obj: firstLevel) {
        if (obj.child_num) obj.offset /= obj.child_num;
    }
}

void PrintProcess(PrintMap treeMap) {
    for (int level = 0; level < treeMap->size(); ++level) {
        int base = treeMap->at(level).front().offset + PERSON_NAME_MAX_LENGTH,
            parent = 0;
        bool first_child = true;
        
        if (level) {
            std::cout << std::string(treeMap->at(level).front().offset + PERSON_NAME_MAX_LENGTH / 2, ' ') << "+";
            for (int col = 1; col < treeMap->at(level).size(); ++col) {
                auto obj =  treeMap->at(level)[col];
                int offset = obj.offset - base > 0 ? obj.offset - base : 0;
                char insertChar = ' ';
                if (obj.parent == parent) {
                    insertChar = '-';
                } else {
                    parent = obj.parent;
                }
                std::cout << std::string(offset + PERSON_NAME_MAX_LENGTH - 1, insertChar) << "+";
                base += offset + PERSON_NAME_MAX_LENGTH;
            }
            std::cout << std::endl;
        }

        base = treeMap->at(level).front().offset + PERSON_NAME_MAX_LENGTH;
        if (level) std::cout << std::string(treeMap->at(level).front().offset + PERSON_NAME_MAX_LENGTH / 2, ' ') << "|    ";
        for (int col = 1; col < treeMap->at(level).size(); ++col) {
            int offset = treeMap->at(level)[col].offset - base;
            if (offset < 0) offset = 0;
            std::cout << std::string(offset + PERSON_NAME_MAX_LENGTH / 2, ' ') << "|    ";
            base += offset + PERSON_NAME_MAX_LENGTH;
        }
        std::cout << std::endl;
        
        base = treeMap->at(level).front().offset + PERSON_NAME_MAX_LENGTH; 
        std::cout << std::string(treeMap->at(level).front().offset, ' ') << treeMap->at(level).front().person;
        for (int col = 1; col < treeMap->at(level).size(); ++col) {
            int offset = treeMap->at(level)[col].offset - base;
            if (offset < 0) offset = 0;
            std::cout << std::string(offset, ' ') << treeMap->at(level)[col].person;
            base += offset + PERSON_NAME_MAX_LENGTH;
        }
        std::cout << std::endl;

        base = treeMap->at(level).front().offset + PERSON_NAME_MAX_LENGTH; 
        std::shared_ptr<const Person> couple = std::dynamic_pointer_cast<const Parent>(treeMap->at(level).front().person)->Couple();
        std::cout << std::string(treeMap->at(level).front().offset, ' ');
        if (couple) std::cout << couple;
        else std::cout << std::string(PERSON_NAME_MAX_LENGTH, ' ');
        for (int col = 1; col < treeMap->at(level).size(); ++col) {
            int offset = treeMap->at(level)[col].offset - base;
            if (offset < 0) offset = 0;
            std::cout << std::string(offset, ' ');
            couple = std::dynamic_pointer_cast<const Parent>(treeMap->at(level)[col].person)->Couple();
            if (couple) std::cout << couple;
            else std::cout << std::string(PERSON_NAME_MAX_LENGTH, ' ');
            base += offset + PERSON_NAME_MAX_LENGTH;
        }
        std::cout << std::endl;

        base = treeMap->at(level).front().offset + PERSON_NAME_MAX_LENGTH; 
        std::cout << std::string(treeMap->at(level).front().offset + PERSON_NAME_MAX_LENGTH / 2, ' ') << (treeMap->at(level).front().child_num ? "|    " : "     ");
        for (int col = 1; col < treeMap->at(level).size(); ++col) {
            int offset = treeMap->at(level)[col].offset - base;
            if (offset < 0) offset = 0;
            std::cout << std::string(offset + PERSON_NAME_MAX_LENGTH / 2, ' ') << (treeMap->at(level)[col].child_num ? "|    " : "     ");
            base += offset + PERSON_NAME_MAX_LENGTH;
        }
        std::cout << std::endl;
    }
}

PrintMap PrintPreProcess(std::shared_ptr<const Person> ancestor) {
    std::queue<std::shared_ptr<const Person>> queue;
    PrintMap treeMap;
    treeMap.reset(new std::vector<std::vector<printStruct>>);
    treeMap->push_back(std::vector<printStruct>());
    treeMap->back().push_back(printStruct(ancestor));
    queue.push(ancestor);
    VisitByLevel(queue, treeMap);
    return treeMap;
}

void printFamilyTree(FamilyTree &tree) {
    auto current_person_save = tree.SelectPerson();
    if (current_person_save == nullptr &&
            tree.GetError() == FamilyTree::Error::RESULT_NOT_FOUND)
        return;
    auto treeMap = PrintPreProcess(getAncestor(tree));
    initOffset(treeMap);
    PrintProcess(treeMap);
    tree.SelectPerson(current_person_save->Id());
}

void printPersonTree(std::shared_ptr<const Person> someone) {
    auto treeMap = PrintPreProcess(someone);
    initOffset(treeMap);
    PrintProcess(treeMap);
}