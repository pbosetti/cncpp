/*
 ____ _____ _       _     _     _
/ ___|_   _| |     | |   (_)___| |_
\___ \ | | | |     | |   | / __| __|
 ___) || | | |___  | |___| \__ \ |_
|____/ |_| |_____| |_____|_|___/\__|

Standard Template Library (STL) List
Author: Davide Stocco, 2025
*/

#include <iostream>
#include <list>
#include <string>

// Struct for list of objects
struct Person {
    std::string name;
    int age;
};

int main() {
    // 1. Declaring and Initializing Lists
    std::list<int> lst1 = {1, 2, 3};
    std::list<int> lst2{4, 5, 6};
    std::list<int> lst3(5, 10); // {10, 10, 10, 10, 10}

    std::list<Person> people = {{"Alice", 25}, {"Bob", 30}};

    // 2. Accessing Elements
    std::cout << "Front: " << lst1.front() << ", Back: " << lst1.back() << std::endl;

    // 3. Iterating through a List
    std::cout << "Iterating (forward): ";
    for (auto it = lst1.begin(); it != lst1.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << "Iterating (reverse): ";
    for (auto it = lst1.rbegin(); it != lst1.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 4. Adding Elements
    lst1.push_back(10);
    lst1.push_front(0);
    lst1.emplace_back(20);
    lst1.emplace_front(-1);

    std::cout << "After adding elements: ";
    for (int num : lst1) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // 5. Inserting at a Specific Position
    auto it = lst1.begin();
    std::advance(it, 3); // Move to 4th element
    lst1.insert(it, 99);

    std::cout << "After insert(99) at 4th position: ";
    for (int num : lst1) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // 6. Removing Elements
    lst1.pop_back();
    lst1.pop_front();
    lst1.remove(3); // Remove all occurrences of 3

    std::cout << "After pop and remove(3): ";
    for (int num : lst1) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // 7. Sorting, Reversing & Removing Duplicates
    lst1.sort();
    lst1.reverse();
    lst1.unique(); // Only works on sorted list

    std::cout << "After sort, reverse, and unique(): ";
    for (int num : lst1) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // 8. Merging Lists
    std::list<int> lst4 = {2, 4, 6};
    std::list<int> lst5 = {1, 3, 5};
    lst4.merge(lst5);

    std::cout << "After merging: ";
    for (int num : lst4) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // 9. Splicing (Moving Elements Between Lists)
    std::list<int> lst6 = {100, 200, 300};
    auto it2 = lst4.begin();
    std::advance(it2, 2);
    lst4.splice(it2, lst6);

    std::cout << "After splicing lst6 into lst4: ";
    for (int num : lst4) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // 10. Capacity Functions
    std::cout << "Size of lst4: " << lst4.size() << std::endl;
    std::cout << "Is lst6 empty? " << (lst6.empty() ? "Yes" : "No") << std::endl;

    lst4.clear();
    std::cout << "After clearing lst4, is it empty? " << (lst4.empty() ? "Yes" : "No") << std::endl;

    return 0;
}
