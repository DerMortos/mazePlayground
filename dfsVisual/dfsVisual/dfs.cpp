#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <chrono>

class TreeNode {
public:
    int value;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int val) : value(val), left(nullptr), right(nullptr) {}
};

class BinaryTree {
public:
    TreeNode* root;

    BinaryTree() : root(nullptr) {}

    void insert(int value) {
        if (root == nullptr) {
            root = new TreeNode(value);
        }
        else {
            insertHelper(root, value);
        }
    }

    void preorderTraversal(TreeNode* node) {
        if (node != nullptr) {
            visit(node);
            preorderTraversal(node->left);
            preorderTraversal(node->right);
        }
    }

    void inorderTraversal(TreeNode* node) {
        if (node != nullptr) {
            inorderTraversal(node->left);
            visit(node);
            inorderTraversal(node->right);
        }
    }

    void postorderTraversal(TreeNode* node) {
        if (node != nullptr) {
            postorderTraversal(node->left);
            postorderTraversal(node->right);
            visit(node);
        }
    }

private:
    void insertHelper(TreeNode* node, int value) {
        if (value < node->value) {
            if (node->left == nullptr) {
                node->left = new TreeNode(value);
            }
            else {
                insertHelper(node->left, value);
            }
        }
        else {
            if (node->right == nullptr) {
                node->right = new TreeNode(value);
            }
            else {
                insertHelper(node->right, value);
            }
        }
    }

    void visit(TreeNode* node) {
        std::cout << "Visiting node with value: " << node->value << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // delay .5 sec
};

void printTree(TreeNode* node, int space = 0, int height = 10) {
    if (node == nullptr) return;
    space += height;

    printTree(node->right, space);
    std::cout << std::endl;
    for (int i = height; i < space; i++) std::cout << ' ';
    std::cout << node->value << "\n";
    printTree(node->left, space);
}

int main() {
    BinaryTree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.insert(3);
    tree.insert(7);
    tree.insert(12);
    tree.insert(18);

    std::cout << "Tree Structure:" << std::endl;
    printTree(tree.root);

    std::cout << "\nPreorder Traversal:" << std::endl;
    tree.preorderTraversal(tree.root);

    std::cout << "\nInorder Traversal:" << std::endl;
    tree.inorderTraversal(tree.root);

    std::cout << "\nPostorder Traversal:" << std::endl;
    tree.postorderTraversal(tree.root);

    return 0;
}
