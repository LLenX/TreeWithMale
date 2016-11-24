#include "utility/PrintTree.hpp"
#include <functional>
#include <vector>
#include <queue>


/*
 * a struct helps to storage some info of the tree node.
 * person -> the pointer to person.
 * parent -> the postion of the parent (father in fact) in the upper level.
 * child_num -> the # of children this person and his wife have.
 * offset -> the offset from the begining of a line to the position of current person. 
 */
struct printStruct {
    using Person_p = std::shared_ptr<const Person>;

    Person_p person;
    int offset, parent, child_num;
    printStruct(Person_p person_, int parent_ = -1)
        : person(person_), parent(parent_), offset(0) {
        child_num =
            std::dynamic_pointer_cast<const Parent>(person)->Children()->size();
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


/*
 * visit by level to traverse the family tree.
 * @param queue The queue of last level.
 * @param treeMap The 2D vector pointer of printStruct contains the
 *                whole tree by level.
 */
void VisitByLevel(std::queue<std::shared_ptr<const Person>>& queue,
                  PrintMap treeMap) {
    if (queue.empty()) return;
    std::queue<std::shared_ptr<const Person>> next_level;
    treeMap->push_back(std::vector<printStruct>());
    int parent_pos = 0;
    while (!queue.empty()) {
        auto front = queue.front();
        for (auto item :
            *std::dynamic_pointer_cast<const Parent>(front)->Children()) {
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


/*
 * init the offset in the treeMap
 */
void initOffset(PrintMap treeMap) {
    int prevOffset = -PERSON_NAME_MAX_LENGTH;
    auto& lastLevel = treeMap->back();
    for (auto& obj: lastLevel) {
        prevOffset = obj.offset = prevOffset + PERSON_NAME_MAX_LENGTH;
    }
    if (treeMap->size() == 1) return;
    for (auto levelIter = treeMap->end() - 1;
        levelIter != treeMap->begin(); --levelIter) {
        for (auto objp = levelIter->begin();
            objp != levelIter->end(); ++objp) {
            if (objp->child_num)
                objp->offset /= objp->child_num;
            else if (objp != levelIter->begin())
                objp->offset = (objp - 1)->offset + PERSON_NAME_MAX_LENGTH;
            (levelIter - 1)->at(objp->parent).offset += objp->offset;
        }
    }
    auto& firstLevel = treeMap->front();
    for (auto& obj: firstLevel) {
        if (obj.child_num) obj.offset /= obj.child_num;
    }
}

using PrintFunction = std::function<void (printStruct&, int&, bool)>;

void printOneLine(std::vector<printStruct>& level,
                  PrintFunction print) {
    if (level.empty()) return;
    int base = level.front().offset + PERSON_NAME_MAX_LENGTH;
    print(level.front(), base, true);
    for (int i = 1; i < level.size(); ++i) {
        print(level[i], base, false);
    }
    std::cout << std::endl;
}

void PrintProcess(PrintMap treeMap) {
    for (int level = 0; level < treeMap->size(); ++level) {
        auto current_level = treeMap->at(level);
        int base = current_level.front().offset + PERSON_NAME_MAX_LENGTH,
            parent = 0;

        /*
         *   1. ->              +------
         *   2. ->              |
         *   3. ->           husband
         *   4. ->             wife
         *   5. ->              |
         *                +----------+
         *                |          |
         *              son 1       son 2
         *       daughter-in-law  daughter-in-law
         */ 
        

        //  ptint the first line
        printOneLine(current_level,
            [&parent](printStruct& printObj, int& base, bool first) ->void {
            if (first) {
                std::cout << std::string(
                                printObj.offset + PERSON_NAME_MAX_LENGTH / 2,
                                ' ') << "+";
            } else {
                int offset = printObj.offset - base;
                if (offset < 0) offset = 0;
                char insertChar = ' ';
                if (parent == printObj.parent) insertChar = '-';
                else parent = printObj.parent;
                std::cout << std::string(offset + PERSON_NAME_MAX_LENGTH - 1, insertChar) << "+";
                base += offset + PERSON_NAME_MAX_LENGTH;
            }
        });

        //  ptint the second line
        printOneLine(current_level,
                    [](printStruct& printObj, int& base, bool first)->void {
            if (first) {
                std::cout << std::string(
                                printObj.offset + PERSON_NAME_MAX_LENGTH / 2, ' ')
                          << '|' << std::string(PERSON_NAME_MAX_LENGTH / 2, ' ');
            } else {
                int offset = printObj.offset - base;
                if (offset < 0) offset = 0;
                std::cout << std::string(offset + PERSON_NAME_MAX_LENGTH / 2, ' ')
                          << '|' << std::string(PERSON_NAME_MAX_LENGTH / 2, ' ');
                base += offset + PERSON_NAME_MAX_LENGTH;
            }
        });

        //  ptint the third line
        printOneLine(current_level,
                    [](printStruct& printObj, int& base, bool first)->void {
            if (first) {
                std::cout <<std::string(printObj.offset, ' ')
                          << printObj.person;
            } else {
                int offset = printObj.offset - base;
                if (offset < 0) offset = 0;
                std::cout << std::string(offset, ' ') << printObj.person;
                base += offset + PERSON_NAME_MAX_LENGTH;
            }
        });
        
        //  ptint the fourth line
        printOneLine(current_level,
                    [](printStruct& printObj, int& base, bool first)->void {
            auto couple
                    = std::dynamic_pointer_cast<const Parent>(
                        printObj.person)->Couple();
            int offset = printObj.offset - base;
            if (offset < 0) offset = 0;
            std::cout << std::string(first ? printObj.offset : offset, ' ');
            if (couple)
                std::cout << std::dynamic_pointer_cast<const Person>(couple);
            else
                std::cout << std::string(PERSON_NAME_MAX_LENGTH, ' ');
            if (!first) base += offset + PERSON_NAME_MAX_LENGTH;
        });

        //  ptint the fifth line
        printOneLine(current_level,
                    [](printStruct& printObj, int& base, bool first) ->void {
            int half = PERSON_NAME_MAX_LENGTH / 2;
            int offset = printObj.offset - base;
            if (offset < 0) offset = 0;
            if (offset < 0) offset = 0;
            std::cout << std::string(offset + half, ' ')
                      << (printObj.child_num ? '|' : ' ')
                      << std::string(half, ' '); 
            if (!first) base += offset + PERSON_NAME_MAX_LENGTH;
        });
    }
}



/*
 * Generate the printStruct Matrix contains whole tree.
 */
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