#include <SplayTree/SplayTree.hpp>

#include <iostream>
#include <string>

int main() {
    SplayTree<int, std::string> tree;

    tree.Insert(2, "two");
    tree.Insert(4, "four");
    tree.Insert(90, "ninety");
    tree.Insert(3, "three");
    tree.Insert(0, "zero");
    tree.Insert(14, "fourteen");
    tree.Insert(45, "forty-five");

    auto result1 = tree.Search(1);
    if (result1) {
        std::cout << result1->key << " -> " << result1->value << std::endl;
    } else {
        std::cout << "Key not found!" << std::endl;
    }

    auto result2 = tree.Search(3);
    if (result2) {
        std::cout << result2->key << " -> " << result2->value << std::endl;
    } else {
        std::cout << "Key not found!" << std::endl;
    }

    return 0;
}