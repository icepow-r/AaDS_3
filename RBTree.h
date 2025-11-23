#ifndef RBTREE_H
#define RBTREE_H

#include <functional>
#include <queue>
#include <stack>

enum Color
{
    RED,
    BLACK
};

template <typename T>
struct RBNode
{
    T data;
    RBNode* left;
    RBNode* right;
    RBNode* parent;
    Color color;

    RBNode(T val) : data(val), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
};

template <typename T>
class RBTree
{
   private:
    RBNode<T>* root;

    void rotateLeft(RBNode<T>* node)
    {
        RBNode<T>* rightChild = node->right;
        node->right = rightChild->left;

        if (rightChild->left != nullptr)
        {
            rightChild->left->parent = node;
        }

        rightChild->parent = node->parent;

        if (node->parent == nullptr)
        {
            root = rightChild;
        }
        else if (node == node->parent->left)
        {
            node->parent->left = rightChild;
        }
        else
        {
            node->parent->right = rightChild;
        }

        rightChild->left = node;
        node->parent = rightChild;
    }

    void rotateRight(RBNode<T>* node)
    {
        RBNode<T>* leftChild = node->left;
        node->left = leftChild->right;

        if (leftChild->right != nullptr)
        {
            leftChild->right->parent = node;
        }

        leftChild->parent = node->parent;

        if (node->parent == nullptr)
        {
            root = leftChild;
        }
        else if (node == node->parent->right)
        {
            node->parent->right = leftChild;
        }
        else
        {
            node->parent->left = leftChild;
        }

        leftChild->right = node;
        node->parent = leftChild;
    }

    void fixInsert(RBNode<T>* node)
    {
        while (node != root && node->parent->color == RED)
        {
            if (node->parent == node->parent->parent->left)
            {
                RBNode<T>* uncle = node->parent->parent->right;

                if (uncle != nullptr && uncle->color == RED)
                {
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                }
                else
                {
                    if (node == node->parent->right)
                    {
                        node = node->parent;
                        rotateLeft(node);
                    }
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    rotateRight(node->parent->parent);
                }
            }
            else
            {
                RBNode<T>* uncle = node->parent->parent->left;

                if (uncle != nullptr && uncle->color == RED)
                {
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                }
                else
                {
                    if (node == node->parent->left)
                    {
                        node = node->parent;
                        rotateRight(node);
                    }
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    rotateLeft(node->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void insertNode(T value)
    {
        RBNode<T>* newNode = new RBNode<T>(value);
        RBNode<T>* parent = nullptr;
        RBNode<T>* current = root;

        while (current != nullptr)
        {
            parent = current;
            if (newNode->data < current->data)
            {
                current = current->left;
            }
            else if (newNode->data > current->data)
            {
                current = current->right;
            }
            else
            {
                delete newNode;
                return;
            }
        }

        newNode->parent = parent;

        if (parent == nullptr)
        {
            root = newNode;
        }
        else if (newNode->data < parent->data)
        {
            parent->left = newNode;
        }
        else
        {
            parent->right = newNode;
        }

        fixInsert(newNode);
    }

    void transplant(RBNode<T>* u, RBNode<T>* v)
    {
        if (u->parent == nullptr)
        {
            root = v;
        }
        else if (u == u->parent->left)
        {
            u->parent->left = v;
        }
        else
        {
            u->parent->right = v;
        }
        if (v != nullptr)
        {
            v->parent = u->parent;
        }
    }

    RBNode<T>* minimum(RBNode<T>* node)
    {
        while (node->left != nullptr)
        {
            node = node->left;
        }
        return node;
    }

    void fixDelete(RBNode<T>* node)
    {
        while (node != root && (node == nullptr || node->color == BLACK))
        {
            if (node == node->parent->left)
            {
                RBNode<T>* sibling = node->parent->right;

                if (sibling != nullptr && sibling->color == RED)
                {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotateLeft(node->parent);
                    sibling = node->parent->right;
                }

                if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
                    (sibling->right == nullptr || sibling->right->color == BLACK))
                {
                    sibling->color = RED;
                    node = node->parent;
                }
                else
                {
                    if (sibling->right == nullptr || sibling->right->color == BLACK)
                    {
                        if (sibling->left != nullptr)
                        {
                            sibling->left->color = BLACK;
                        }
                        sibling->color = RED;
                        rotateRight(sibling);
                        sibling = node->parent->right;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    if (sibling->right != nullptr)
                    {
                        sibling->right->color = BLACK;
                    }
                    rotateLeft(node->parent);
                    node = root;
                }
            }
            else
            {
                RBNode<T>* sibling = node->parent->left;

                if (sibling != nullptr && sibling->color == RED)
                {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotateRight(node->parent);
                    sibling = node->parent->left;
                }

                if ((sibling->right == nullptr || sibling->right->color == BLACK) &&
                    (sibling->left == nullptr || sibling->left->color == BLACK))
                {
                    sibling->color = RED;
                    node = node->parent;
                }
                else
                {
                    if (sibling->left == nullptr || sibling->left->color == BLACK)
                    {
                        if (sibling->right != nullptr)
                        {
                            sibling->right->color = BLACK;
                        }
                        sibling->color = RED;
                        rotateLeft(sibling);
                        sibling = node->parent->left;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    if (sibling->left != nullptr)
                    {
                        sibling->left->color = BLACK;
                    }
                    rotateRight(node->parent);
                    node = root;
                }
            }
        }
        if (node != nullptr)
        {
            node->color = BLACK;
        }
    }

    void deleteNode(RBNode<T>* node)
    {
        RBNode<T>* y = node;
        RBNode<T>* x;
        Color yOriginalColor = y->color;

        if (node->left == nullptr)
        {
            x = node->right;
            transplant(node, node->right);
        }
        else if (node->right == nullptr)
        {
            x = node->left;
            transplant(node, node->left);
        }
        else
        {
            y = minimum(node->right);
            yOriginalColor = y->color;
            x = y->right;

            if (y->parent == node)
            {
                if (x != nullptr)
                {
                    x->parent = y;
                }
            }
            else
            {
                transplant(y, y->right);
                y->right = node->right;
                y->right->parent = y;
            }

            transplant(node, y);
            y->left = node->left;
            y->left->parent = y;
            y->color = node->color;
        }

        delete node;

        if (yOriginalColor == BLACK && x != nullptr)
        {
            fixDelete(x);
        }
    }

    RBNode<T>* searchNode(RBNode<T>* node, T value)
    {
        if (node == nullptr || node->data == value)
        {
            return node;
        }

        if (value < node->data)
        {
            return searchNode(node->left, value);
        }
        else
        {
            return searchNode(node->right, value);
        }
    }

    void destroyTree(RBNode<T>* node)
    {
        if (node)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

   public:
    RBTree() : root(nullptr) {}

    ~RBTree() { destroyTree(root); }

    void insert(T value) { insertNode(value); }

    void remove(T value)
    {
        RBNode<T>* node = searchNode(root, value);
        if (node != nullptr)
        {
            deleteNode(node);
        }
    }

    bool search(T value) { return searchNode(root, value) != nullptr; }

    void breadthFirstTraversal(std::function<void(T)> visit)
    {
        if (!root) return;

        std::queue<RBNode<T>*> q;
        q.push(root);

        while (!q.empty())
        {
            RBNode<T>* current = q.front();
            q.pop();

            visit(current->data);

            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
    }

    void preorderTraversal(std::function<void(T)> visit)
    {
        if (!root) return;

        std::stack<RBNode<T>*> s;
        s.push(root);

        while (!s.empty())
        {
            RBNode<T>* current = s.top();
            s.pop();

            visit(current->data);

            if (current->right) s.push(current->right);
            if (current->left) s.push(current->left);
        }
    }

    void inorderTraversal(std::function<void(T)> visit)
    {
        if (!root) return;

        std::stack<RBNode<T>*> s;
        RBNode<T>* current = root;

        while (current || !s.empty())
        {
            while (current)
            {
                s.push(current);
                current = current->left;
            }

            current = s.top();
            s.pop();

            visit(current->data);

            current = current->right;
        }
    }

    void postorderTraversal(std::function<void(T)> visit)
    {
        if (!root) return;

        std::stack<RBNode<T>*> s1, s2;
        s1.push(root);

        while (!s1.empty())
        {
            RBNode<T>* current = s1.top();
            s1.pop();
            s2.push(current);

            if (current->left) s1.push(current->left);
            if (current->right) s1.push(current->right);
        }

        while (!s2.empty())
        {
            visit(s2.top()->data);
            s2.pop();
        }
    }

    RBNode<T>* getRoot() const { return root; }

    void breadthFirstTraversalWithColor(std::function<void(T, Color)> visit)
    {
        if (!root) return;

        std::queue<RBNode<T>*> q;
        q.push(root);

        while (!q.empty())
        {
            RBNode<T>* current = q.front();
            q.pop();

            visit(current->data, current->color);

            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
    }

    void preorderTraversalWithColor(std::function<void(T, Color)> visit)
    {
        if (!root) return;

        std::stack<RBNode<T>*> s;
        s.push(root);

        while (!s.empty())
        {
            RBNode<T>* current = s.top();
            s.pop();

            visit(current->data, current->color);

            if (current->right) s.push(current->right);
            if (current->left) s.push(current->left);
        }
    }

    void inorderTraversalWithColor(std::function<void(T, Color)> visit)
    {
        if (!root) return;

        std::stack<RBNode<T>*> s;
        RBNode<T>* current = root;

        while (current || !s.empty())
        {
            while (current)
            {
                s.push(current);
                current = current->left;
            }

            current = s.top();
            s.pop();

            visit(current->data, current->color);

            current = current->right;
        }
    }

    void postorderTraversalWithColor(std::function<void(T, Color)> visit)
    {
        if (!root) return;

        std::stack<RBNode<T>*> s1, s2;
        s1.push(root);

        while (!s1.empty())
        {
            RBNode<T>* current = s1.top();
            s1.pop();
            s2.push(current);

            if (current->left) s1.push(current->left);
            if (current->right) s1.push(current->right);
        }

        while (!s2.empty())
        {
            RBNode<T>* node = s2.top();
            visit(node->data, node->color);
            s2.pop();
        }
    }
};

#endif