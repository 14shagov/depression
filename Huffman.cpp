#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
using namespace std;

struct HuffmanNode 
{
    char character;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;
    
    HuffmanNode(char ch, int freq) : character(ch), frequency(freq), left(nullptr), right(nullptr) {}
    HuffmanNode(int freq) : character('\0'), frequency(freq), left(nullptr), right(nullptr) {}
};

struct CompareNodes 
{
    bool operator()(HuffmanNode* first, HuffmanNode* second) 
    {
        return first->frequency > second->frequency;
    }
};

void generateHuffmanCodes(HuffmanNode* root, string currentCode, unordered_map<char, string>& encodingTable) 
{
    if (!root) return;
    
    if (!root->left && !root->right) 
    {
        encodingTable[root->character] = currentCode;
        return;
    }
    
    generateHuffmanCodes(root->left, currentCode + "0", encodingTable);
    generateHuffmanCodes(root->right, currentCode + "1", encodingTable);
}

void printHuffmanTree(HuffmanNode* root, int indentLevel = 0) 
{
    if (!root) return;
    
    indentLevel += 4;
    printHuffmanTree(root->right, indentLevel);
    
    cout << endl;
    for (int i = 4; i < indentLevel; i++) 
    {
        cout << " ";
    }
    
    if (root->character == '\0')
        cout << root->frequency << endl;
    else
        cout << root->character << ":" << root->frequency << endl;
        
    printHuffmanTree(root->left, indentLevel);
}

int main() 
{
    string inputPhrase;
    cout << "Введите фразу: ";
    getline(cin, inputPhrase);
    
    if (inputPhrase.empty()) 
    {
        cout << "Ошибка: пустой ввод!" << endl;
        return 0;
    }

    // 1. Подсчёт частот символов
    unordered_map<char, int> frequencyMap;
    for (char symbol : inputPhrase) 
    {
        frequencyMap[symbol]++;
    }
    
    // 2. Создание приоритетной очереди
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNodes> priorityQueue;
    for (auto& entry : frequencyMap) 
    {
        priorityQueue.push(new HuffmanNode(entry.first, entry.second));
    }
    
    // 3. Построение дерева Хаффмана
    while (priorityQueue.size() > 1) 
    {
        HuffmanNode* leftChild = priorityQueue.top(); 
        priorityQueue.pop();
        
        HuffmanNode* rightChild = priorityQueue.top(); 
        priorityQueue.pop();
        
        HuffmanNode* internalNode = new HuffmanNode(leftChild->frequency + rightChild->frequency);
        internalNode->left = leftChild;
        internalNode->right = rightChild;
        priorityQueue.push(internalNode);
    }
    
    HuffmanNode* rootNode = priorityQueue.top();
    unordered_map<char, string> huffmanCodes;
    
    // 4. Генерация кодов
    if (frequencyMap.size() == 1)
        huffmanCodes[rootNode->character] = "0";
    else
        generateHuffmanCodes(rootNode, "", huffmanCodes);
    
    // 5. Вывод результатов
    cout << "\nВизуализация дерева Хаффмана:\n";
    printHuffmanTree(rootNode);
    
    cout << "\nТаблица кодов:\n";
    for (auto& entry : huffmanCodes) 
    {
        cout << "'" << entry.first << "': " << entry.second << endl;
    }
    
    delete rootNode;
    return 0;
}
/*

```

**Что делает программа:**

1. **Анализ входных данных**:
- Получает строку от пользователя
- Проверяет ввод на пустоту

2. **Подсчёт частот**:
- Создает частотный словарь (символ → количество повторений)
- Пример: для "abracadabra" получим a:5, b:2, r:2, c:1, d:1

3. **Построение очереди**:
- Создает узлы для каждого символа
- Организует приоритетную очередь по частоте (минимальная частота - высший приоритет)

4. **Сборка дерева**:
- Извлекает два узла с наименьшими частотами
- Создает новый родительский удел с суммой частот
- Повторяет до полного построения дерева

5. **Генерация кодов**:
- Обходит дерево (левый переход = 0, правый = 1)
- Формирует таблицу кодирования для каждого символа

6. **Визуализация**:
- Рисует вертикальное дерево с отступами
- Выводит таблицу соответствия символов и их кодов

**Ключевые особенности**:
- Автоматическая оптимизация кодов под частоту символов
- Эффективное использование памяти через указатели
- Рекурсивные алгоритмы для обхода дерева
- Поддержка специальныхр случаев (один символ в строке)
- Наглядное представление структуры данных

*/
