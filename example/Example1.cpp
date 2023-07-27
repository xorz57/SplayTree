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

    tree.PreOrderTraversal([](auto key, auto &value) {
        std::cout << key << " -> " << value << std::endl;
    });
    std::cout << std::endl;

    tree.InOrderTraversal([](auto key, auto &value) {
        std::cout << key << " -> " << value << std::endl;
    });
    std::cout << std::endl;

    tree.PostOrderTraversal([](auto key, auto &value) {
        std::cout << key << " -> " << value << std::endl;
    });
    std::cout << std::endl;

    return 0;
}