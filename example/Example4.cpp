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

    std::cout << "Height: " << tree.Height() << std::endl;
    std::cout << "Size: " << tree.Size() << std::endl;

    tree.Clear();

    std::cout << "Height: " << tree.Height() << std::endl;
    std::cout << "Size: " << tree.Size() << std::endl;

    return 0;
}