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
    std::cout << (isLeft ? "├── " : "└── ");
    std::cout << node->data << "\n";

    if (node->left || node->right)
    {
        if (node->left)
        {
            printBinaryTree(node->left, prefix + (isLeft ? "│   " : "    "), true);
        }
        else if (node->right)
        {
            std::cout << prefix << (isLeft ? "│   " : "    ") << "├── (empty)\n";
        }

        if (node->right)
        {
            printBinaryTree(node->right, prefix + (isLeft ? "│   " : "    "), false);
        }
    }
}

void printRBTreeHelper(RBNode<int>* node, std::string prefix = "", bool isLeft = true)
{
    if (!node) return;

    std::cout << prefix;
    std::cout << (isLeft ? "├── " : "└── ");
    std::cout << node->data;
    std::cout << (node->color == RED ? "(R)" : "(B)");
    std::cout << "\n";

    if (node->left || node->right)
    {
        if (node->left)
        {
            printRBTreeHelper(node->left, prefix + (isLeft ? "│   " : "    "), true);
        }
        else if (node->right)
        {
            std::cout << prefix << (isLeft ? "│   " : "    ") << "├── (empty)\n";
        }

        if (node->right)
        {
            printRBTreeHelper(node->right, prefix + (isLeft ? "│   " : "    "), false);
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
            std::cout << "\n╔════════════════════════════════════════╗\n";
            std::cout << "║         Loading Tree from File         ║\n";
            std::cout << "╚════════════════════════════════════════╝\n";
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
            std::cout << "\n⚠ No tree loaded. Please load a tree first.\n";
            return;
        }

        std::cout << "\n╔════════════════════════════════════════╗\n";
        std::cout << "║     Binary Tree Visualization          ║\n";
        std::cout << "╚════════════════════════════════════════╝\n";
        std::cout << "\nRoot\n";
        printBinaryTree(binaryTree->getRoot(), "", true);
    }

    void visualizeRBTree()
    {
        if (!treeLoaded)
        {
            std::cout << "\n⚠ No tree loaded. Please load a tree first.\n";
            return;
        }

        std::cout << "\n╔════════════════════════════════════════╗\n";
        std::cout << "║   Red-Black Tree Visualization         ║\n";
        std::cout << "╚════════════════════════════════════════╝\n";
        std::cout << "\n(R) = Red, (B) = Black\n";
        std::cout << "\nRoot\n";
        printRBTreeHelper(rbTree->getRoot(), "", true);
    }

    void traverseBinaryTree()
    {
        if (!treeLoaded)
        {
            std::cout << "\n⚠ No tree loaded. Please load a tree first.\n";
            return;
        }

        std::cout << "\n╔════════════════════════════════════════╗\n";
        std::cout << "║  Binary Tree Traversal (Preorder)      ║\n";
        std::cout << "╚════════════════════════════════════════╝\n";
        std::cout << "\nNodes: ";
        bool first = true;
        binaryTree->traverse(
            [&first](int val)
            {
                if (!first) std::cout << " → ";
                std::cout << val;
                first = false;
            });
        std::cout << "\n";
    }

    void traverseRBTree()
    {
        if (!treeLoaded)
        {
            std::cout << "\n⚠ No tree loaded. Please load a tree first.\n";
            return;
        }

        std::cout << "\n╔════════════════════════════════════════╗\n";
        std::cout << "║    Red-Black Tree Traversal Menu       ║\n";
        std::cout << "╚════════════════════════════════════════╝\n";
        std::cout << "\n1. Breadth-First (Level Order)\n";
        std::cout << "2. Depth-First Preorder\n";
        std::cout << "3. Depth-First Inorder (Sorted)\n";
        std::cout << "4. Depth-First Postorder\n";
        std::cout << "\nYour choice: ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\nInvalid input!\n";
            return;
        }

        std::cout << "\n═══════════════════════════════════════\n";
        std::cout << "Nodes: ";
        bool first = true;

        switch (choice)
        {
            case 1:
                std::cout << "(Breadth-First)\n";
                rbTree->breadthFirstTraversal(
                    [&first](int val)
                    {
                        if (!first) std::cout << " → ";
                        std::cout << val;
                        first = false;
                    });
                break;
            case 2:
                std::cout << "(Preorder)\n";
                rbTree->preorderTraversal(
                    [&first](int val)
                    {
                        if (!first) std::cout << " → ";
                        std::cout << val;
                        first = false;
                    });
                break;
            case 3:
                std::cout << "(Inorder - Sorted)\n";
                rbTree->inorderTraversal(
                    [&first](int val)
                    {
                        if (!first) std::cout << " → ";
                        std::cout << val;
                        first = false;
                    });
                break;
            case 4:
                std::cout << "(Postorder)\n";
                rbTree->postorderTraversal(
                    [&first](int val)
                    {
                        if (!first) std::cout << " → ";
                        std::cout << val;
                        first = false;
                    });
                break;
            default:
                std::cout << "\nInvalid choice!\n";
                return;
        }
        std::cout << "\n";
    }
};


void printMenu()
{
    std::cout << "╔════════════════════════════════════════╗\n";
    std::cout << "║ 1. Load tree from file                 ║\n";
    std::cout << "║ 2. Visualize Binary Tree               ║\n";
    std::cout << "║ 3. Visualize Red-Black Tree            ║\n";
    std::cout << "║ 4. Traverse Binary Tree                ║\n";
    std::cout << "║ 5. Traverse Red-Black Tree             ║\n";
    std::cout << "║ 0. Exit                                ║\n";
    std::cout << "╚════════════════════════════════════════╝\n";
}

int main()
{
    TreeManager manager;
    int choice;

    std::cout << "\n╔════════════════════════════════════════╗\n";
    std::cout << "║  Binary & Red-Black Tree Visualizer    ║\n";
    std::cout << "╚════════════════════════════════════════╝\n";

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
                std::cout << "\n╔════════════════════════════════════════╗\n";
                std::cout << "║          Exiting program...            ║\n";
                std::cout << "║            Goodbye!                    ║\n";
                std::cout << "╚════════════════════════════════════════╝\n\n";
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

            default:
                std::cout << "\nInvalid choice! Please try again.\n";
        }
    }
}