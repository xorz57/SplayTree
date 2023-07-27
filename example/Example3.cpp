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

    auto min = tree.Minimum();
    if (min) {
        std::cout << "Min Key: " << min->key << std::endl;
    } else {
        std::cout << "The tree is empty!" << std::endl;
    }

    auto max = tree.Maximum();
    if (max) {
        std::cout << "Max Key: " << max->key << std::endl;
    } else {
        std::cout << "The tree is empty!" << std::endl;
    }

    return 0;
}