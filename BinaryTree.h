#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <functional>

template <typename T>
struct BinaryTreeNode
{
    T data;
    BinaryTreeNode* left;
    BinaryTreeNode* right;

    BinaryTreeNode(T val) : data(val), left(nullptr), right(nullptr) {}
};

template <typename T>
class BinaryTree
{
   private:
    BinaryTreeNode<T>* root;

    void destroyTree(BinaryTreeNode<T>* node)
    {
        if (node)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    void preorderTraversal(BinaryTreeNode<T>* node, std::function<void(T)> visit)
    {
        if (node)
        {
            visit(node->data);
            preorderTraversal(node->left, visit);
            preorderTraversal(node->right, visit);
        }
    }

   public:
    BinaryTree() : root(nullptr) {}

    ~BinaryTree() { destroyTree(root); }

    void setRoot(BinaryTreeNode<T>* node) { root = node; }

    BinaryTreeNode<T>* getRoot() const { return root; }

    void traverse(std::function<void(T)> visit) { preorderTraversal(root, visit); }
};

#endif