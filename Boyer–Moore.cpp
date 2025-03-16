#include <iostream>
#include <vector>
#include <unordered_map>
#include <clocale>
#include <algorithm>
#include <string>
#define NOMINMAX
#include <windows.h>

void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Таблица плохих символов (корректная реализация)
std::unordered_map<char, int> BuildBadCharTable(const std::string& pattern)
{
    std::unordered_map<char, int> badChar;
    int len = pattern.length();
    for (int i = 0; i < len; ++i)
        badChar[pattern[i]] = len - i - 1; // Последнее вхождение символа
    return badChar;
}

/*

Цель: Для каждого символа паттерна сохранить его последнюю позицию от конца.

Пример: Для паттерна "ABABC":

'A' → 4, 'B' → 3, 'C' → 0.

Использование: При несовпадении символа в тексте сдвигает паттерн на соответствующее значение.

*/

// Корректная реализация таблицы хороших суффиксов
std::vector<int> BuildGoodSuffixTable(const std::string& pattern)
{
    int m = pattern.length();
    std::vector<int> shift(m + 1, m);
    std::vector<int> borders(m + 1, 0);

    // Фаза 1: Поиск границ суффиксов
    int i = m;
    int j = m + 1;
    borders[i] = j;
    while (i > 0)
    {
        while (j <= m && (i == 0 || pattern[i - 1] != pattern[j - 1]))
        {
            if (shift[j] == m)
                shift[j] = j - i;
            j = borders[j];
        }
        i--;
        j--;
        borders[i] = j;
    }

    // Фаза 2: Уточнение сдвигов
    j = borders[0];
    for (i = 0; i <= m; ++i)
    {
        if (shift[i] == m)
            shift[i] = j;
        if (i == j)
            j = borders[j];
    }

    return shift;
}

/*
Цель: Определить, на сколько можно сдвинуть паттерн при совпадении суффикса.

Фаза 1: Находит границы совпадающих суффиксов.

Фаза 2: Корректирует сдвиги для перекрывающихся префиксов и суффиксов.
*/

std::vector<int> BoyerMooreSearch(const std::string& text, const std::string& pattern)
{
    std::vector<int> matches;
    int n = text.length();
    int m = pattern.length();
    if (m == 0 || n < m) return matches;

    auto badChar = BuildBadCharTable(pattern);
    auto goodSuffix = BuildGoodSuffixTable(pattern);
    int shift = 0;

    while (shift <= n - m) {
        int j = m - 1;

        // Сравнение справа налево
        while (j >= 0 && pattern[j] == text[shift + j])
            j--;

        if (j < 0)
        {
            matches.push_back(shift);
            shift += goodSuffix[0]; // Сдвиг при полном совпадении
        }
        else
        {
            // Сдвиг по правилу плохого символа
            char bc = text[shift + j];
            int bcShift = badChar.count(bc) ? badChar[bc] : -1;
            int badCharShift = (bcShift != -1) ? (j - bcShift) : (m - j);
            badCharShift = std::max(1, badCharShift);

            // Сдвиг по правилу хорошего суффикса
            int goodSuffixShift = goodSuffix[j + 1];

            // Выбираем максимальный сдвиг
            shift += std::max(badCharShift, goodSuffixShift);
        }
    }
    return matches;
}

/*
Сравнение справа налево: Уменьшает количество сравнений.

Правило плохого символа: Сдвигает паттерн на максимальное значение из таблицы.

Правило хорошего суффикса: Учитывает совпадение суффикса для большего сдвига.


*/

void main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    const int DEFAULT_COLOR = 7;

    std::string text, pattern;
    std::cout << "Введите текст: ";
    std::getline(std::cin, text);
    std::cout << "Введите паттерн: ";
    std::getline(std::cin, pattern);

    if (pattern.empty())
    {
        std::cout << "Ошибка: паттерн не может быть пустым." << std::endl;
        return;
    }

    std::vector<int> matches = BoyerMooreSearch(text, pattern);

    if (matches.empty())
    {
        std::cout << "Совпадений не найдено." << std::endl;
        return;
    }

    // Создание и сортировка событий для подсветки
    std::vector<std::pair<int, char>> events;
    for (int start : matches)
    {
        events.emplace_back(start, 's');
        events.emplace_back(start + pattern.length(), 'e');
    }

    std::sort(events.begin(), events.end(), [](const auto& a, const auto& b)
        {
        if (a.first == b.first) return a.second > b.second;
        return a.first < b.first;
        });

    // Вывод текста с подсветкой
    size_t eventIndex = 0;
    bool inColor = false;

    std::cout << "\nРезультат поиска:\n";
    for (int i = 0; i < text.length(); ++i)
    {
        while (eventIndex < events.size() && events[eventIndex].first == i)
        {
            if (events[eventIndex].second == 's')
            {
                SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
                inColor = true;
            }
            else
            {
                SetColor(DEFAULT_COLOR);
                inColor = false;
            }
            ++eventIndex;
        }
        std::cout << text[i];
    }

    if (inColor)
        SetColor(DEFAULT_COLOR);

    std::cout << "\nСовпадения найдены на позициях: ";
    for (int pos : matches) std::cout << pos << " ";
    std::cout << std::endl;

    return;
}

/*
Ввод данных: Считывает текст и паттерн с поддержкой русских символов.

Обработка событий: Создает и сортирует события для корректной подсветки.

Вывод: Отображает текст с выделением совпадений красным цветом и выводит позиции.
*/
