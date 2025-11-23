#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "BinaryTree.h"
#include "Parser.h"
#include "RBTree.h"

std::string readFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void printBinaryTree(BinaryTreeNode<int>* node, std::string prefix = "", bool isLeft = true)
{
    if (!node) return;

    std::cout << prefix;
    std::cout << (isLeft ? "|-- " : "|-- ");
    std::cout << node->data << "\n";

    if (node->left || node->right)
    {
        if (node->left)
        {
            printBinaryTree(node->left, prefix + (isLeft ? "|   " : "    "), true);
        }
        else if (node->right)
        {
            std::cout << prefix << (isLeft ? "|   " : "    ") << "|-- (empty)\n";
        }

        if (node->right)
        {
            printBinaryTree(node->right, prefix + (isLeft ? "|   " : "    "), false);
        }
    }
}

void printRBTreeHelper(RBNode<int>* node, std::string prefix = "", bool isLeft = true)
{
    if (!node) return;

    std::cout << prefix;
    std::cout << (isLeft ? "|-- " : "|-- ");
    std::cout << node->data;
    std::cout << (node->color == RED ? "(R)" : "(B)");
    std::cout << "\n";

    if (node->left || node->right)
    {
        if (node->left)
        {
            printRBTreeHelper(node->left, prefix + (isLeft ? "|   " : "    "), true);
        }
        else if (node->right)
        {
            std::cout << prefix << (isLeft ? "|   " : "    ") << "|-- (empty)\n";
        }

        if (node->right)
        {
            printRBTreeHelper(node->right, prefix + (isLeft ? "|   " : "    "), false);
        }
    }
}

class TreeManager
{
private:
    std::unique_ptr<BinaryTree<int>> binaryTree;
    std::unique_ptr<RBTree<int>> rbTree;
    bool treeLoaded;
    std::string currentFile;

public:
    TreeManager() : treeLoaded(false)
    {
    }

    void loadFromFile(const std::string& filename)
    {
        try
        {
            std::string content = readFile(filename);
            std::cout << "        Loading Tree from File         \n";
            std::cout << "\nFile: " << filename << "\n";
            std::cout << "Content: " << content << "\n";

            Parser<int> parser(content);
            BinaryTreeNode<int>* treeRoot = parser.parse();

            binaryTree = std::make_unique<BinaryTree<int>>();
            binaryTree->setRoot(treeRoot);

            rbTree = std::make_unique<RBTree<int>>();
            binaryTree->traverse([this](int val) { rbTree->insert(val); });

            treeLoaded = true;
            currentFile = filename;

            std::cout << "\nBinary tree successfully loaded!\n";
            std::cout << "Red-Black tree created from binary tree!\n";
        }
        catch (const std::exception& e)
        {
            std::cout << "\nError loading tree: " << e.what() << "\n";
            treeLoaded = false;
        }
    }

    void visualizeBinaryTree()
    {
        if (!treeLoaded)
        {
            std::cout << "\nNo tree loaded. Please load a tree first.\n";
            return;
        }

        std::cout << "    Binary Tree Visualization          \n";
        std::cout << "\nRoot\n";
        printBinaryTree(binaryTree->getRoot(), "", true);
    }

    void visualizeRBTree()
    {
        if (!treeLoaded)
        {
            std::cout << "\nNo tree loaded. Please load a tree first.\n";
            return;
        }

        std::cout << "  Red-Black Tree Visualization         \n";
        std::cout << "\n(R) = Red, (B) = Black\n";
        std::cout << "\nRoot\n";
        printRBTreeHelper(rbTree->getRoot(), "", true);
    }

    void traverseBinaryTree()
    {
        if (!treeLoaded)
        {
            std::cout << "\nNo tree loaded. Please load a tree first.\n";
            return;
        }

        std::cout << " Binary Tree Traversal (Preorder)      \n";
        std::cout << "\nNodes: ";
        bool first = true;
        binaryTree->traverse(
            [&first](int val)
            {
                if (!first) std::cout << " -> ";
                std::cout << val;
                first = false;
            });
        std::cout << "\n";
    }

    void traverseRBTree()
    {
        if (!treeLoaded)
        {
            std::cout << "\nNo tree loaded. Please load a tree first.\n";
            return;
        }

        std::cout << "   Red-Black Tree All Traversals       \n";

        std::cout << "\nInorder (Sorted): ";
        bool first = true;
        rbTree->inorderTraversalWithColor(
            [&first](int val, Color color)
            {
                if (!first) std::cout << " -> ";
                std::cout << val << (color == RED ? "(R)" : "(B)");
                first = false;
            });

        std::cout << "\n\nPreorder: ";
        first = true;
        rbTree->preorderTraversalWithColor(
            [&first](int val, Color color)
            {
                if (!first) std::cout << " -> ";
                std::cout << val << (color == RED ? "(R)" : "(B)");
                first = false;
            });

        std::cout << "\n\nPostorder: ";
        first = true;
        rbTree->postorderTraversalWithColor(
            [&first](int val, Color color)
            {
                if (!first) std::cout << " -> ";
                std::cout << val << (color == RED ? "(R)" : "(B)");
                first = false;
            });

        std::cout << "\n\nBreadth-First (Level Order): ";
        first = true;
        rbTree->breadthFirstTraversalWithColor(
            [&first](int val, Color color)
            {
                if (!first) std::cout << " -> ";
                std::cout << val << (color == RED ? "(R)" : "(B)");
                first = false;
            });
        std::cout << "\n";
    }

    void insertToRBTree()
    {
        if (!treeLoaded)
        {
            std::cout << "\nNo tree loaded. Please load a tree first.\n";
            return;
        }

        std::cout << "\nEnter value to insert: ";
        int value;
        std::cin >> value;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\nInvalid input!\n";
            return;
        }

        rbTree->insert(value);
        std::cout << "\nValue " << value << " successfully inserted into Red-Black tree!\n";
    }

    void deleteFromRBTree()
    {
        if (!treeLoaded)
        {
            std::cout << "\nNo tree loaded. Please load a tree first.\n";
            return;
        }

        std::cout << "\nEnter value to delete: ";
        int value;
        std::cin >> value;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\nInvalid input!\n";
            return;
        }

        if (rbTree->search(value))
        {
            rbTree->remove(value);
            std::cout << "\nValue " << value << " successfully deleted from Red-Black tree!\n";
        }
        else
        {
            std::cout << "\nValue " << value << " not found in tree!\n";
        }
    }

    void searchInRBTree()
    {
        if (!treeLoaded)
        {
            std::cout << "\nNo tree loaded. Please load a tree first.\n";
            return;
        }

        std::cout << "\nEnter value to search: ";
        int value;
        std::cin >> value;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\nInvalid input!\n";
            return;
        }

        if (rbTree->search(value))
        {
            std::cout << "\nValue " << value << " FOUND in Red-Black tree!\n";
        }
        else
        {
            std::cout << "\nValue " << value << " NOT FOUND in Red-Black tree!\n";
        }
    }
};


void printMenu()
{
    std::cout << " 1. Load tree from file                 \n";
    std::cout << " 2. Visualize Binary Tree               \n";
    std::cout << " 3. Visualize Red-Black Tree            \n";
    std::cout << " 4. Traverse Binary Tree                \n";
    std::cout << " 5. Traverse Red-Black Tree (All)       \n";
    std::cout << " 6. Insert element to RB Tree           \n";
    std::cout << " 7. Delete element from RB Tree         \n";
    std::cout << " 8. Search element in RB Tree           \n";
    std::cout << " 0. Exit                                \n";
}

int main()
{
    TreeManager manager;
    int choice;

    std::cout << "\nBinary & Red-Black Tree Visualizer    \n";

    while (true)
    {
        printMenu();
        std::cout << "\nEnter your choice: ";
        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\nInvalid input! Please enter a number.\n";
            continue;
        }

        switch (choice)
        {
            case 0:
                std::cout << "         Exiting program...            \n";
                std::cout << "           Goodbye!                    \n";
                return 0;

            case 1:
            {
                std::cout << "\nEnter filename: ";
                std::string filename;
                std::cin >> filename;
                manager.loadFromFile(filename);
                break;
            }

            case 2:
                manager.visualizeBinaryTree();
                break;

            case 3:
                manager.visualizeRBTree();
                break;

            case 4:
                manager.traverseBinaryTree();
                break;

            case 5:
                manager.traverseRBTree();
                break;

            case 6:
                manager.insertToRBTree();
                break;

            case 7:
                manager.deleteFromRBTree();
                break;

            case 8:
                manager.searchInRBTree();
                break;

            default:
                std::cout << "\nInvalid choice! Please try again.\n";
        }
    }
}