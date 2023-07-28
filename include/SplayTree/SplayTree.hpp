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
#include <queue>
#include <utility>

template<typename Key, typename Value>
class SplayTree {
public:
    using Handler = std::function<void(const Key &, Value &)>;

private:
    struct SplayTreeNode {
        SplayTreeNode() = default;

        SplayTreeNode(const Key &key, const Value &value) : key(key), value(value) {}

        ~SplayTreeNode() = default;

        SplayTreeNode *lChild = nullptr;
        SplayTreeNode *rChild = nullptr;

        Key key;
        Value value;
    };

    void PreOrderTraversalHelper(SplayTreeNode *root, Handler handler) {
        if (!root) return;
        handler(root->key, root->value);
        PreOrderTraversalHelper(root->lChild, handler);
        PreOrderTraversalHelper(root->rChild, handler);
    }

    void InOrderTraversalHelper(SplayTreeNode *root, Handler handler) {
        if (!root) return;
        InOrderTraversalHelper(root->lChild, handler);
        handler(root->key, root->value);
        InOrderTraversalHelper(root->rChild, handler);
    }

    void PostOrderTraversalHelper(SplayTreeNode *root, Handler handler) {
        if (!root) return;
        PostOrderTraversalHelper(root->lChild, handler);
        PostOrderTraversalHelper(root->rChild, handler);
        handler(root->key, root->value);
    }

    void ClearHelper(SplayTreeNode *root) {
        if (!root) return;
        if (root->lChild) ClearHelper(root->lChild);
        if (root->rChild) ClearHelper(root->rChild);
        delete root;
    }

    SplayTreeNode *InsertHelper(SplayTreeNode *root, const Key &key, const Value &value) {
        if (root == nullptr) {
            return new SplayTreeNode(key, value);
        }

        root = Splay(root, key);

        if (key < root->key) {
            auto tmp = new SplayTreeNode(key, value);
            tmp->rChild = root;
            tmp->lChild = root->lChild;
            root->lChild = nullptr;
            return tmp;
        } else if (key > root->key) {
            auto tmp = new SplayTreeNode(key, value);
            tmp->lChild = root;
            tmp->rChild = root->rChild;
            root->rChild = nullptr;
            return tmp;
        }

        return root;
    }

    SplayTreeNode *MinimumHelper(SplayTreeNode *root) {
        if (!root) return nullptr;
        while (root->lChild) root = root->lChild;
        return root;
    }

    SplayTreeNode *MaximumHelper(SplayTreeNode *root) {
        if (!root) return nullptr;
        while (root->rChild) root = root->rChild;
        return root;
    }

    SplayTreeNode *RotateRight(SplayTreeNode *root) {
        auto tmp = root->lChild;
        root->lChild = tmp->rChild;
        tmp->rChild = root;
        return tmp;
    }

    SplayTreeNode *RotateLeft(SplayTreeNode *root) {
        auto tmp = root->rChild;
        root->rChild = tmp->lChild;
        tmp->lChild = root;
        return tmp;
    }

    SplayTreeNode *Splay(SplayTreeNode *root, const Key &key) {
        if (root == nullptr || root->key == key) {
            return root;
        }

        if (key < root->key) {
            if (root->lChild == nullptr) {
                return root;
            }

            if (key < root->lChild->key) {
                root->lChild->lChild = Splay(root->lChild->lChild, key);
                root = RotateRight(root);
            } else if (key > root->lChild->key) {
                root->lChild->rChild = Splay(root->lChild->rChild, key);
                if (root->lChild->rChild != nullptr) {
                    root->lChild = RotateLeft(root->lChild);
                }
            }

            return (root->lChild == nullptr) ? root : RotateRight(root);
        } else {
            if (root->rChild == nullptr) {
                return root;
            }

            if (key < root->rChild->key) {
                root->rChild->lChild = Splay(root->rChild->lChild, key);
                if (root->rChild->lChild != nullptr) {
                    root->rChild = RotateRight(root->rChild);
                }
            } else if (key > root->rChild->key) {
                root->rChild->rChild = Splay(root->rChild->rChild, key);
                root = RotateLeft(root);
            }

            return (root->rChild == nullptr) ? root : RotateLeft(root);
        }
    }

    int HeightHelper(const SplayTreeNode *root) {
        if (!root) return 0;
        return std::max(HeightHelper(root->lChild), HeightHelper(root->rChild)) + 1;
    }

    int SizeHelper(const SplayTreeNode *root) {
        if (!root) return 0;
        return SizeHelper(root->lChild) + SizeHelper(root->rChild) + 1;
    }

    SplayTreeNode *mRoot = nullptr;

public:
    SplayTree() = default;

    ~SplayTree() {
        Clear();
    }

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
        ClearHelper(mRoot);
        mRoot = nullptr;
    }

    void Insert(const Key &key, const Value &value) {
        mRoot = InsertHelper(mRoot, key, value);
    }

    SplayTreeNode *Search(const Key &key) {
        return Splay(mRoot, key);
    }

    SplayTreeNode *Minimum() {
        return MinimumHelper(mRoot);
    }

    SplayTreeNode *Maximum() {
        return MaximumHelper(mRoot);
    }

    int Height() {
        return HeightHelper(mRoot);
    }

    int Size() {
        return SizeHelper(mRoot);
    }
};