#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <chrono>

// a more elaborate representation of the binary DFS using Simple and Fast Multimedia Library package available through
// https://vcpkg.io/en/index.html

class TreeNode {
public:
    int value;
    TreeNode* left;
    TreeNode* right;
    sf::CircleShape circle;
    sf::Text text;

    TreeNode(int val, sf::Font& font) : value(val), left(nullptr), right(nullptr) {
        circle.setRadius(20);
        circle.setFillColor(sf::Color::White);
        circle.setOutlineColor(sf::Color::Black);
        circle.setOutlineThickness(2);
        circle.setOrigin(circle.getRadius(), circle.getRadius());

        text.setFont(font);
        text.setString(std::to_string(value));
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);
        text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
    }
};

class BinaryTree {
public:
    TreeNode* root;
    sf::Font font;

    BinaryTree() : root(nullptr) {
        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
        }
    }

    void insert(int value) {
        if (root == nullptr) {
            root = new TreeNode(value, font);
        }
        else {
            insertHelper(root, value);
        }
    }

    void traverse(std::function<void(TreeNode*)> visit) {
        if (root != nullptr) {
            traverseHelper(root, visit);
        }
    }

private:
    void insertHelper(TreeNode* node, int value) {
        if (value < node->value) {
            if (node->left == nullptr) {
                node->left = new TreeNode(value, font);
            }
            else {
                insertHelper(node->left, value);
            }
        }
        else {
            if (node->right == nullptr) {
                node->right = new TreeNode(value, font);
            }
            else {
                insertHelper(node->right, value);
            }
        }
    }

    void traverseHelper(TreeNode* node, std::function<void(TreeNode*)> visit) {
        if (node != nullptr) {
            visit(node);
            traverseHelper(node->left, visit);
            traverseHelper(node->right, visit);
        }
    }
};

void drawTree(TreeNode* node, sf::RenderWindow& window, float x, float y, float horizontalOffset) {
    if (node == nullptr) return;

    node->circle.setPosition(x, y);
    node->text.setPosition(x, y);

    if (node->left != nullptr) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x, y)),
            sf::Vertex(sf::Vector2f(x - horizontalOffset, y + 100))
        };
        window.draw(line, 2, sf::Lines);
        drawTree(node->left, window, x - horizontalOffset, y + 100, horizontalOffset / 2);
    }

    if (node->right != nullptr) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x, y)),
            sf::Vertex(sf::Vector2f(x + horizontalOffset, y + 100))
        };
        window.draw(line, 2, sf::Lines);
        drawTree(node->right, window, x + horizontalOffset, y + 100, horizontalOffset / 2);
    }

    window.draw(node->circle);
    window.draw(node->text);
}

void updateDisplay(sf::RenderWindow& window, BinaryTree& tree, const std::string& traversalType, const std::string& stepInfo) {
    window.clear(sf::Color::White);

    // Draw the traversal type and step info
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }

    sf::Text traversalText(traversalType, font, 20);
    traversalText.setFillColor(sf::Color::Black);
    traversalText.setPosition(10, 10);

    sf::Text stepText(stepInfo, font, 20);
    stepText.setFillColor(sf::Color::Black);
    stepText.setPosition(10, 40);

    window.draw(traversalText);
    window.draw(stepText);

    // Draw the tree
    drawTree(tree.root, window, window.getSize().x / 2, 100, 200);
    window.display();
}

void animateTraversal(TreeNode* node, sf::RenderWindow& window, BinaryTree& tree, const std::string& traversalType, std::function<void(TreeNode*)> visit) {
    if (node == nullptr) return;

    visit(node);

    node->circle.setFillColor(sf::Color::Yellow);
    updateDisplay(window, tree, traversalType, "Visiting node: " + std::to_string(node->value));
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    animateTraversal(node->left, window, tree, traversalType, visit);
    animateTraversal(node->right, window, tree, traversalType, visit);

    node->circle.setFillColor(sf::Color::White);
    updateDisplay(window, tree, traversalType, "Backtracking from node: " + std::to_string(node->value));
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Binary Tree Traversal");

    BinaryTree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.insert(3);
    tree.insert(7);
    tree.insert(12);
    tree.insert(18);

    bool runPreorder = true, runInorder = false, runPostorder = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (runPreorder) {
            animateTraversal(tree.root, window, tree, "Preorder Traversal", [](TreeNode* node) {
                std::cout << "Preorder visit: " << node->value << std::endl;
                });
            runPreorder = false;
            runInorder = true;
        }
        else if (runInorder) {
            animateTraversal(tree.root, window, tree, "Inorder Traversal", [](TreeNode* node) {
                std::cout << "Inorder visit: " << node->value << std::endl;
                });
            runInorder = false;
            runPostorder = true;
        }
        else if (runPostorder) {
            animateTraversal(tree.root, window, tree, "Postorder Traversal", [](TreeNode* node) {
                std::cout << "Postorder visit: " << node->value << std::endl;
                });
            runPostorder = false;
        }

        updateDisplay(window, tree, "Waiting", "");
    }

    return 0;
}
