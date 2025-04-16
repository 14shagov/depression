#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>

#define NOMINMAX
#include <windows.h>

#define DEFAULT_COLOR 7

void SetConsoleColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

std::unordered_map<char, int> BuildBadCharTable(const std::string& pattern)
{
    std::unordered_map<char, int> badChar;
    int len = pattern.length();
    for (int i = 0; i < len; ++i)
        badChar[pattern[i]] = i;
    return badChar;
}

std::vector<int> BuildGoodSuffixTable(const std::string& pattern)
{
    int m = pattern.length();
    std::vector<int> shift(m + 1, m);
    std::vector<int> borders(m + 1, 0);

    int suffixPos = m;
    int prefixPtr = m + 1;
    borders[suffixPos] = prefixPtr;

    while (suffixPos > 0)
    {
        while (prefixPtr <= m && pattern[suffixPos - 1] != pattern[prefixPtr - 1])
        {
            if (shift[prefixPtr] == m)
                shift[prefixPtr] = prefixPtr - suffixPos;
            prefixPtr = borders[prefixPtr];
        }
        suffixPos--;
        prefixPtr--;
        borders[suffixPos] = prefixPtr;
    }

    int currentBorder = borders[0];
    for (int shiftPos = 0; shiftPos <= m; ++shiftPos)
    {
        if (shift[shiftPos] == m)
            shift[shiftPos] = currentBorder;
        if (shiftPos == currentBorder)
            currentBorder = borders[currentBorder];
    }

    return shift;
}

std::vector<int> BoyerMooreSearch(const std::string& text, const std::string& pattern)
{
    std::vector<int> matches;
    int n = text.length();
    int m = pattern.length();
    if (m == 0 || n < m)
        return matches;

    auto badChar = BuildBadCharTable(pattern);
    auto goodSuffix = BuildGoodSuffixTable(pattern);
    int shift = 0;

    while (shift <= n - m)
    {
        int patternIndex = m - 1;
        while (patternIndex >= 0 && pattern[patternIndex] == text[shift + patternIndex])
            patternIndex--;

        if (patternIndex < 0)
        {
            matches.push_back(shift);
            shift += goodSuffix[0];
        }
        else
        {
            char mismatchChar = text[shift + patternIndex];
            int bcShift = badChar.count(mismatchChar) ? badChar[mismatchChar] : -1;
            int badCharShift = std::max(1, patternIndex - bcShift);
            int goodSuffixShift = goodSuffix[patternIndex + 1];
            shift += std::max(badCharShift, goodSuffixShift);
        }
    }
    return matches;
}

std::vector<std::pair<int, int>> MergeIntervals(const std::vector<int>& positions, int patternLen)
{
    std::vector<std::pair<int, int>> merged;
    if (positions.empty())
        return merged;

    std::vector<int> sorted = positions;
    std::sort(sorted.begin(), sorted.end());

    int start = sorted[0];
    int end = start + patternLen;

    for (size_t i = 1; i < sorted.size(); ++i)
    {
        int pos = sorted[i];
        if (pos <= end)
            end = std::max(end, pos + patternLen);
        else
        {
            merged.emplace_back(start, end);
            start = pos;
            end = pos + patternLen;
        }
    }
    merged.emplace_back(start, end);

    return merged;
}

void ColorRendering(const std::string& text, const std::vector<std::pair<int, int>>& merged) 
{
    std::cout << std::endl << "Результат поиска : " << std::endl;
    SetConsoleColor(DEFAULT_COLOR);

    size_t currentInterval = 0;
    bool isHighlighted = false;

    for (size_t i = 0; i < text.size(); ++i) 
    {
        if (currentInterval < merged.size() && i == merged[currentInterval].first) 
        {
            SetConsoleColor(FOREGROUND_RED);
            isHighlighted = true;
        }

        std::cout << text[i];

        if (isHighlighted && i + 1 == merged[currentInterval].second) 
        {
            SetConsoleColor(DEFAULT_COLOR);
            isHighlighted = false;
            currentInterval++;
        }
    }
    
    if (isHighlighted) 
        SetConsoleColor(DEFAULT_COLOR);
}

void ToLower(std::string& text)
{
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c)
        {
            return std::tolower(c);
        });
}

void main()
{
    setlocale(LC_ALL, "RU");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::string text, pattern;
    std::cout << "Введите текст: ";
    std::getline(std::cin, text);
    std::cout << "Введите паттерн: ";
    std::getline(std::cin, pattern);
    std::string saveText = text;

    ToLower(text);
    ToLower(pattern);

    if (text.empty())
    {
        std::cout << "Ошибка: текст не может быть пустым" << std::endl;
        return;
    }

    if (pattern.empty())
    {
        std::cout << "Ошибка: паттерн не может быть пустым" << std::endl;
        return;
    }

    std::vector<int> matches = BoyerMooreSearch(text, pattern);
    std::vector<std::pair<int, int>> merged = MergeIntervals(matches, pattern.length());

    if (merged.empty())
    {
        std::cout << "Совпадений не найдено" << std::endl;
        return;
    }

    ColorRendering(saveText, merged);

    std::cout << std::endl << "Совпадения: ";
    for (const auto& interval : merged)
        std::cout << "[" << interval.first << "-" << interval.second - 1 << "] ";
    std::cout << std::endl;
}
