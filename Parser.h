#ifndef PARSER_H
#define PARSER_H

#include <cctype>
#include <stdexcept>
#include <string>

#include "BinaryTree.h"

template <typename T>
class Parser
{
   private:
    std::string input;
    size_t pos;

    void skipWhitespace()
    {
        while (pos < input.length() && std::isspace(input[pos]))
        {
            pos++;
        }
    }

    T parseNumber()
    {
        skipWhitespace();

        if (pos >= input.length())
        {
            throw std::runtime_error("Unexpected end of input");
        }

        bool negative = false;
        if (input[pos] == '-')
        {
            negative = true;
            pos++;
        }

        if (pos >= input.length() || !std::isdigit(input[pos]))
        {
            throw std::runtime_error("Expected number");
        }

        T value = 0;
        while (pos < input.length() && std::isdigit(input[pos]))
        {
            value = value * 10 + (input[pos] - '0');
            pos++;
        }

        return negative ? -value : value;
    }

    BinaryTreeNode<T>* parseNode()
    {
        skipWhitespace();

        if (pos >= input.length())
        {
            throw std::runtime_error("Unexpected end of input");
        }

        if (input[pos] != '(')
        {
            throw std::runtime_error("Expected '('");
        }
        pos++;

        T value = parseNumber();
        BinaryTreeNode<T>* node = new BinaryTreeNode<T>(value);

        int childCount = 0;

        while (true)
        {
            skipWhitespace();

            if (pos >= input.length())
            {
                throw std::runtime_error("Unexpected end of input");
            }

            if (input[pos] == ')')
            {
                pos++;
                break;
            }

            if (input[pos] == '(')
            {
                childCount++;

                if (childCount > 2)
                {
                    throw std::runtime_error("More than two children (not a binary tree)");
                }

                if (childCount == 1)
                {
                    node->left = parseNode();
                }
                else
                {
                    node->right = parseNode();
                }
            }
            else
            {
                throw std::runtime_error("Expected '(' or ')'");
            }
        }

        return node;
    }

    void validateInput()
    {
        for (char c : input)
        {
            if (!std::isspace(c) && !std::isdigit(c) && c != '(' && c != ')' && c != '-')
            {
                throw std::runtime_error("Invalid character in input");
            }
        }

        int balance = 0;
        for (char c : input)
        {
            if (c == '(')
                balance++;
            else if (c == ')')
                balance--;
            if (balance < 0)
            {
                throw std::runtime_error("Unbalanced parentheses");
            }
        }
        if (balance != 0)
        {
            throw std::runtime_error("Unbalanced parentheses");
        }
    }

   public:
    Parser(const std::string& str) : input(str), pos(0) {}

    BinaryTreeNode<T>* parse()
    {
        validateInput();

        skipWhitespace();

        if (input.empty() || pos >= input.length())
        {
            throw std::runtime_error("Empty input");
        }

        BinaryTreeNode<T>* root = parseNode();

        skipWhitespace();

        if (pos < input.length())
        {
            throw std::runtime_error("Extra characters after tree");
        }

        return root;
    }
};

#endif