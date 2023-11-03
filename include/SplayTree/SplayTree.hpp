/*
	MIT License

	Copyright(c) 2017 George Fotopoulos

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files(the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions :

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#pragma once

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <memory>
#include <queue>
#include <utility>

template<typename Key, typename Value>
class SplayTree {
public:
    using Handler = std::function<void(const Key &, Value &)>;

private:
    struct SplayTreeNode {
        SplayTreeNode(const Key &key, const Value &value) : key(key), value(value) {}

        std::shared_ptr<SplayTreeNode> lChild = nullptr;
        std::shared_ptr<SplayTreeNode> rChild = nullptr;

        Key key;
        Value value;
    };

    void PreOrderTraversalHelper(std::shared_ptr<SplayTreeNode> root, Handler handler) {
        if (!root) return;
        handler(root->key, root->value);
        PreOrderTraversalHelper(root->lChild, handler);
        PreOrderTraversalHelper(root->rChild, handler);
    }

    void InOrderTraversalHelper(std::shared_ptr<SplayTreeNode> root, Handler handler) {
        if (!root) return;
        InOrderTraversalHelper(root->lChild, handler);
        handler(root->key, root->value);
        InOrderTraversalHelper(root->rChild, handler);
    }

    void PostOrderTraversalHelper(std::shared_ptr<SplayTreeNode> root, Handler handler) {
        if (!root) return;
        PostOrderTraversalHelper(root->lChild, handler);
        PostOrderTraversalHelper(root->rChild, handler);
        handler(root->key, root->value);
    }

    std::shared_ptr<SplayTreeNode> InsertHelper(std::shared_ptr<SplayTreeNode> root, const Key &key, const Value &value) {
        if (!root) return std::make_shared<SplayTreeNode>(key, value);
        root = Splay(root, key);
        if (key < root->key) {
            auto temp = std::make_shared<SplayTreeNode>(key, value);
            temp->rChild = root;
            temp->lChild = root->lChild;
            root->lChild = nullptr;
            return temp;
        } else if (key > root->key) {
            auto temp = std::make_shared<SplayTreeNode>(key, value);
            temp->lChild = root;
            temp->rChild = root->rChild;
            root->rChild = nullptr;
            return temp;
        }
        return root;
    }

    std::shared_ptr<SplayTreeNode> RemoveHelper(std::shared_ptr<SplayTreeNode> root, const Key &key) {
        if (!root) return nullptr;
        root = Splay(root, key);
        if (root->key != key) return root;
        auto temp = root;
        if (!root->lChild) {
            root = root->rChild;
        } else if (!root->rChild) {
            root = root->lChild;
        } else {
            root = Splay(root->lChild, key);
            root->rChild = temp->rChild;
        }
        temp.reset();
        return root;
    }

    std::shared_ptr<SplayTreeNode> MinimumHelper(std::shared_ptr<SplayTreeNode> root) {
        if (!root) return nullptr;
        while (root->lChild) root = root->lChild;
        return root;
    }

    std::shared_ptr<SplayTreeNode> MaximumHelper(std::shared_ptr<SplayTreeNode> root) {
        if (!root) return nullptr;
        while (root->rChild) root = root->rChild;
        return root;
    }

    std::shared_ptr<SplayTreeNode> RotateRight(std::shared_ptr<SplayTreeNode> root) {
        auto temp = root->lChild;
        root->lChild = temp->rChild;
        temp->rChild = root;
        return temp;
    }

    std::shared_ptr<SplayTreeNode> RotateLeft(std::shared_ptr<SplayTreeNode> root) {
        auto temp = root->rChild;
        root->rChild = temp->lChild;
        temp->lChild = root;
        return temp;
    }

    std::shared_ptr<SplayTreeNode> Splay(std::shared_ptr<SplayTreeNode> root, const Key &key) {
        if (!root) return nullptr;
        if (root->key == key) return root;
        if (key < root->key) {
            if (!root->lChild) return root;
            if (key < root->lChild->key) {
                root->lChild->lChild = Splay(root->lChild->lChild, key);
                root = RotateRight(root);
            } else if (key > root->lChild->key) {
                root->lChild->rChild = Splay(root->lChild->rChild, key);
                if (root->lChild->rChild) {
                    root->lChild = RotateLeft(root->lChild);
                }
            }
            return (root->lChild) ? RotateRight(root) : root;
        } else {
            if (!root->rChild) return root;
            if (key < root->rChild->key) {
                root->rChild->lChild = Splay(root->rChild->lChild, key);
                if (root->rChild->lChild) {
                    root->rChild = RotateRight(root->rChild);
                }
            } else if (key > root->rChild->key) {
                root->rChild->rChild = Splay(root->rChild->rChild, key);
                root = RotateLeft(root);
            }
            return (root->rChild) ? RotateLeft(root) : root;
        }
    }

    int HeightHelper(const std::shared_ptr<SplayTreeNode> root) {
        if (!root) return 0;
        return std::max(HeightHelper(root->lChild), HeightHelper(root->rChild)) + 1;
    }

    int SizeHelper(const std::shared_ptr<SplayTreeNode> root) {
        if (!root) return 0;
        return SizeHelper(root->lChild) + SizeHelper(root->rChild) + 1;
    }

    std::shared_ptr<SplayTreeNode> mRoot;

public:
    SplayTree() = default;

    void PreOrderTraversal(Handler handler) {
        PreOrderTraversalHelper(mRoot, handler);
    }

    void InOrderTraversal(Handler handler) {
        InOrderTraversalHelper(mRoot, handler);
    }

    void PostOrderTraversal(Handler handler) {
        PostOrderTraversalHelper(mRoot, handler);
    }

    void Clear() {
        mRoot.reset();
    }

    void Insert(const Key &key, const Value &value) {
        mRoot = InsertHelper(mRoot, key, value);
    }

    void Remove(const Key &key) {
        mRoot = RemoveHelper(mRoot, key);
    }

    std::shared_ptr<SplayTreeNode> Search(const Key &key) {
        mRoot = Splay(mRoot, key);
        return (mRoot->key == key) ? mRoot : nullptr;
    }

    std::shared_ptr<SplayTreeNode> Minimum() {
        return MinimumHelper(mRoot);
    }

    std::shared_ptr<SplayTreeNode> Maximum() {
        return MaximumHelper(mRoot);
    }

    int Height() {
        return HeightHelper(mRoot);
    }

    int Size() {
        return SizeHelper(mRoot);
    }
};
