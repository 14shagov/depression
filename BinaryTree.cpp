#include <iostream>
#include <random>
#include <limits>
#include <cstdlib>
#include <locale>
#include <sstream>
#include <vector>
#include <algorithm>

class BinaryTree
{
private:
    struct Node
    {
        int data;
        Node* left;
        Node* right;
        Node(int d) : data(d), left(nullptr), right(nullptr) {}
    };

    Node* root;

    Node* insertRec(Node* node, int data)
    {
        if (node == nullptr)
        {
            return new Node(data);
        }
        if (data < node->data)
        {
            node->left = insertRec(node->left, data);
        }
        else if (data > node->data)
        {
            node->right = insertRec(node->right, data);
        }
        return node;
    }

    void printTree(Node* node, int space, int height) const
    {
        if (node == nullptr) return;
        space += height;

        printTree(node->right, space, height);
        std::cout << std::endl;

        for (int i = height; i < space; ++i)
            std::cout << ' ';
        std::cout << node->data << std::endl;

        printTree(node->left, space, height);
    }

    void deleteTree(Node* node)
    {
        if (node)
        {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

    void collectInOrder(Node* node, std::vector<int>& elements)
    {
        if (node == nullptr) return;
        collectInOrder(node->left, elements);
        elements.push_back(node->data);
        collectInOrder(node->right, elements);
    }

    Node* buildBalanced(const std::vector<int>& elements, int start, int end)
    {
        if (start > end) return nullptr;
        int mid = start + (end - start) / 2;
        Node* node = new Node(elements[mid]);
        node->left = buildBalanced(elements, start, mid - 1);
        node->right = buildBalanced(elements, mid + 1, end);
        return node;
    }

public:
    BinaryTree() : root(nullptr) {}

    ~BinaryTree()
    {
        deleteTree(root);
    }

    void insert(int data)
    {
        root = insertRec(root, data);
    }

    void clear()
    {
        deleteTree(root);
        root = nullptr;
    }

    void optimize()
    {
        std::vector<int> elements;
        collectInOrder(root, elements);
        clear();
        root = buildBalanced(elements, 0, elements.size() - 1);
    }

    void fillManual()
    {
        std::cout << "Вводите числа через пробел и нажмите Enter для завершения:" << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string line;
        std::getline(std::cin, line);
        std::stringstream ss(line);
        int num;
        while (ss >> num)
        {
            insert(num);
        }
    }

    void fillRandom(int count)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, 99);
        for (int i = 0; i < count; ++i)
        {
            insert(dist(gen));
        }
    }

    void print() const
    {
        std::cout << std::endl << "Дерево:" << std::endl;
        printTree(root, 0, 10);
        std::cout << std::endl;
    }
};

void main()
{
    setlocale(LC_ALL, "RU");
    BinaryTree tree;
    bool choice;

    std::cout << "Выберите способ заполнения дерева:" << std::endl
        << "Вручную - 0" << std::endl
        << "Случайными числами - 1" << std::endl;
    std::cin >> choice;

    if (!choice)
        tree.fillManual();
    else if (choice)
    {
        int count;
        std::cout << "Введите количество случайных элементов: ";
        std::cin >> count;
        tree.fillRandom(count);
    }
    else
    {
        std::cout << "Неверный выбор." << std::endl;
        return;
    }

    tree.optimize();
    tree.print();
    return;
}
