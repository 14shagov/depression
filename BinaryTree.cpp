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
    // Структура узла дерева
    struct Node
    {
        int data;     // Значение узла
        Node* left;   // Левый потомок
        Node* right;  // Правый потомок
        Node(int d) : data(d), left(nullptr), right(nullptr) {}
    };

    Node* root; // Корень дерева

    // Рекурсивная вставка элемента в дерево
    Node* insertRec(Node* node, int data)
    {
        if (node == nullptr)
        {
            return new Node(data); // Создаём новый узел, если достигли конца ветви
        }
        if (data < node->data)
        {
            node->left = insertRec(node->left, data); // Вставляем в левое поддерево
        }
        else if (data > node->data)
        {
            node->right = insertRec(node->right, data); // Вставляем в правое поддерево
        }
        return node; // Возвращаем текущий узел (дубликаты игнорируются)
    }

    // Рекурсивный вывод дерева в консоль
    void printTree(Node* node, int space, int height) const
    {
        if (node == nullptr) return;
        space += height; // Увеличиваем отступ для визуализации уровней

        printTree(node->right, space, height); // Сначала правые ветви (верх в консоли)
        std::cout << std::endl;

        // Формируем отступ для текущего узла
        for (int i = height; i < space; ++i)
            std::cout << ' ';
        std::cout << node->data << std::endl;

        printTree(node->left, space, height); // Затем левые ветви (низ в консоли)
    }

    // Рекурсивное удаление всех узлов дерева
    void deleteTree(Node* node)
    {
        if (node)
        {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

    // Сбор элементов дерева в отсортированном порядке (in-order)
    void collectInOrder(Node* node, std::vector<int>& elements)
    {
        if (node == nullptr) return;
        collectInOrder(node->left, elements);  // Сначала левое поддерево
        elements.push_back(node->data);        // Текущий узел
        collectInOrder(node->right, elements); // Затем правое поддерево
    }

    // Построение сбалансированного дерева из отсортированного массива
    Node* buildBalanced(const std::vector<int>& elements, int start, int end)
    {
        if (start > end) return nullptr;
        int mid = start + (end - start) / 2;       // Выбираем средний элемент
        Node* node = new Node(elements[mid]);      // Создаём узел
        node->left = buildBalanced(elements, start, mid - 1);  // Рекурсия для левой половины
        node->right = buildBalanced(elements, mid + 1, end);   // Рекурсия для правой половины
        return node;
    }

public:
    BinaryTree() : root(nullptr) {}

    ~BinaryTree()
    {
        deleteTree(root);
    }

    // Вставка элемента в дерево
    void insert(int data)
    {
        root = insertRec(root, data);
    }

    // Очистка дерева
    void clear()
    {
        deleteTree(root);
        root = nullptr;
    }

    // Оптимизация дерева: перестройка в сбалансированное
    void optimize()
    {
        std::vector<int> elements;
        collectInOrder(root, elements); // Собираем элементы
        clear();                        // Удаляем старое дерево
        root = buildBalanced(elements, 0, elements.size() - 1); // Строим новое
    }

    // Ручной ввод элементов (завершается по Enter)
    void fillManual()
    {
        std::cout << "Вводите числа через пробел и нажмите Enter для завершения:" << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера
        std::string line;
        std::getline(std::cin, line); // Чтение всей строки
        std::stringstream ss(line);   // Парсинг чисел из строки
        int num;
        while (ss >> num)
        {
            insert(num);
        }
    }

    // Заполнение случайными числами
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

    // Вывод дерева в консоль
    void print() const
    {
        std::cout << std::endl << "Дерево:" << std::endl;
        printTree(root, 0, 10); // Начальный отступ 0, шаг отступа 10
        std::cout << std::endl;
    }
};

void main()
{
    setlocale(LC_ALL, "RU"); // Для поддержки кириллицы
    BinaryTree tree;
    bool choice;

    // Выбор способа заполнения
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

    tree.optimize(); // Балансировка дерева
    tree.print();    // Вывод результата
    return;
}
