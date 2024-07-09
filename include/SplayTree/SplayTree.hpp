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

template<typename key_t, typename value_t>
struct splay_tree_node_t {
    splay_tree_node_t(const key_t &key, const value_t &value) : key(key), value(value) {}

    std::shared_ptr<splay_tree_node_t> lchild = nullptr;
    std::shared_ptr<splay_tree_node_t> rchild = nullptr;

    key_t key;
    value_t value;
};

template<typename key_t, typename value_t>
using handler_t = std::function<void(const key_t &, value_t &)>;

template<typename key_t, typename value_t>
class splay_tree_t {
private:
    void pre_order_traversal_helper(std::shared_ptr<splay_tree_node_t<key_t, value_t>> root, handler_t<key_t, value_t> handler) {
        if (!root) return;
        handler(root->key, root->value);
        pre_order_traversal_helper(root->lchild, handler);
        pre_order_traversal_helper(root->rchild, handler);
    }

    void in_order_traversal_helper(std::shared_ptr<splay_tree_node_t<key_t, value_t>> root, handler_t<key_t, value_t> handler) {
        if (!root) return;
        in_order_traversal_helper(root->lchild, handler);
        handler(root->key, root->value);
        in_order_traversal_helper(root->rchild, handler);
    }

    void post_order_traversal_helper(std::shared_ptr<splay_tree_node_t<key_t, value_t>> root, handler_t<key_t, value_t> handler) {
        if (!root) return;
        post_order_traversal_helper(root->lchild, handler);
        post_order_traversal_helper(root->rchild, handler);
        handler(root->key, root->value);
    }

    std::shared_ptr<splay_tree_node_t<key_t, value_t>> insert_helper(std::shared_ptr<splay_tree_node_t<key_t, value_t>> root, const key_t &key, const value_t &value) {
        if (!root) return std::make_shared<splay_tree_node_t<key_t, value_t>>(key, value);
        root = splay(root, key);
        if (key < root->key) {
            auto temp = std::make_shared<splay_tree_node_t<key_t, value_t>>(key, value);
            temp->rchild = root;
            temp->lchild = root->lchild;
            root->lchild = nullptr;
            return temp;
        } else if (key > root->key) {
            auto temp = std::make_shared<splay_tree_node_t<key_t, value_t>>(key, value);
            temp->lchild = root;
            temp->rchild = root->rchild;
            root->rchild = nullptr;
            return temp;
        }
        return root;
    }

    std::shared_ptr<splay_tree_node_t<key_t, value_t>> remove_helper(std::shared_ptr<splay_tree_node_t<key_t, value_t>> root, const key_t &key) {
        if (!root) return nullptr;
        root = splay(root, key);
        if (root->key != key) return root;
        auto temp = root;
        if (!root->lchild) {
            root = root->rchild;
        } else if (!root->rchild) {
            root = root->lchild;
        } else {
            root = splay(root->lchild, key);
            root->rchild = temp->rchild;
        }
        temp.reset();
        return root;
    }

    std::shared_ptr<splay_tree_node_t<key_t, value_t>> minimum_helper(std::shared_ptr<splay_tree_node_t<key_t, value_t>> root) {
        if (!root) return nullptr;
        while (root->lchild) root = root->lchild;
        return root;
    }

    std::shared_ptr<splay_tree_node_t<key_t, value_t>> maximum_helper(std::shared_ptr<splay_tree_node_t<key_t, value_t>> root) {
        if (!root) return nullptr;
        while (root->rchild) root = root->rchild;
        return root;
    }

    std::shared_ptr<splay_tree_node_t<key_t, value_t>> rrotate(std::shared_ptr<splay_tree_node_t<key_t, value_t>> root) {
        auto temp = root->lchild;
        root->lchild = temp->rchild;
        temp->rchild = root;
        return temp;
    }

    std::shared_ptr<splay_tree_node_t<key_t, value_t>> lrotate(std::shared_ptr<splay_tree_node_t<key_t, value_t>> root) {
        auto temp = root->rchild;
        root->rchild = temp->lchild;
        temp->lchild = root;
        return temp;
    }

    std::shared_ptr<splay_tree_node_t<key_t, value_t>> splay(std::shared_ptr<splay_tree_node_t<key_t, value_t>> root, const key_t &key) {
        if (!root) return nullptr;
        if (root->key == key) return root;
        if (key < root->key) {
            if (!root->lchild) return root;
            if (key < root->lchild->key) {
                root->lchild->lchild = splay(root->lchild->lchild, key);
                root = rrotate(root);
            } else if (key > root->lchild->key) {
                root->lchild->rchild = splay(root->lchild->rchild, key);
                if (root->lchild->rchild) {
                    root->lchild = lrotate(root->lchild);
                }
            }
            return (root->lchild) ? rrotate(root) : root;
        } else {
            if (!root->rchild) return root;
            if (key < root->rchild->key) {
                root->rchild->lchild = splay(root->rchild->lchild, key);
                if (root->rchild->lchild) {
                    root->rchild = rrotate(root->rchild);
                }
            } else if (key > root->rchild->key) {
                root->rchild->rchild = splay(root->rchild->rchild, key);
                root = lrotate(root);
            }
            return (root->rchild) ? lrotate(root) : root;
        }
    }

    int height_helper(const std::shared_ptr<splay_tree_node_t<key_t, value_t>> root) {
        if (!root) return 0;
        return std::max(height_helper(root->lchild), height_helper(root->rchild)) + 1;
    }

    int size_helper(const std::shared_ptr<splay_tree_node_t<key_t, value_t>> root) {
        if (!root) return 0;
        return size_helper(root->lchild) + size_helper(root->rchild) + 1;
    }

    std::shared_ptr<splay_tree_node_t<key_t, value_t>> m_root;

public:
    splay_tree_t() = default;

    void pre_order_traversal(handler_t<key_t, value_t> handler) {
        pre_order_traversal_helper(m_root, handler);
    }

    void in_order_traversal(handler_t<key_t, value_t> handler) {
        in_order_traversal_helper(m_root, handler);
    }

    void post_order_traversal(handler_t<key_t, value_t> handler) {
        post_order_traversal_helper(m_root, handler);
    }

    void clear() {
        m_root.reset();
    }

    void insert(const key_t &key, const value_t &value) {
        m_root = insert_helper(m_root, key, value);
    }

    void remove(const key_t &key) {
        m_root = remove_helper(m_root, key);
    }

    std::shared_ptr<splay_tree_node_t<key_t, value_t>> search(const key_t &key) {
        m_root = splay(m_root, key);
        return (m_root->key == key) ? m_root : nullptr;
    }

    std::shared_ptr<splay_tree_node_t<key_t, value_t>> minimum() {
        return minimum_helper(m_root);
    }

    std::shared_ptr<splay_tree_node_t<key_t, value_t>> maximum() {
        return maximum_helper(m_root);
    }

    int height() {
        return height_helper(m_root);
    }

    int size() {
        return size_helper(m_root);
    }
};
