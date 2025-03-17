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
        badChar[pattern[i]] = len - i - 1;
    return badChar;
}

std::vector<int> BuildGoodSuffixTable(const std::string& pattern)
{
    int m = pattern.length();
    std::vector<int> shift(m + 1, m);
    std::vector<int> borders(m + 1, 0);

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

    j = borders[0];
    for (i = 0; i <= m; ++i)
    {
        if (shift[i] == m) shift[i] = j;
        if (i == j) j = borders[j];
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
        int j = m - 1;
        while (j >= 0 && pattern[j] == text[shift + j])
            j--;

        if (j < 0)
        {
            matches.push_back(shift);
            shift += goodSuffix[0];
        }
        else
        {
            char bc = text[shift + j];
            int bcShift = badChar.count(bc) ? badChar[bc] : -1;
            int badCharShift = std::max(1, j - bcShift);
            int goodSuffixShift = goodSuffix[j + 1];
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

    std::vector<int> sortedPos = positions;
    std::sort(sortedPos.begin(), sortedPos.end());

    int start = sortedPos[0];
    int end = start + patternLen;

    for (size_t i = 1; i < sortedPos.size(); ++i)
    {
        int pos = sortedPos[i];
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

void ColorRendering(std::string text, std::vector<std::pair<int, int>> merged)
{
    std::vector<std::pair<int, bool>> events;
    for (const auto& interval : merged)
    {
        events.emplace_back(interval.first, true);
        events.emplace_back(interval.second, false);
    }

    std::sort(events.begin(), events.end(), [](const auto& a, const auto& b)
        {
            return a.first < b.first || (a.first == b.first && a.second);
        });

    int highlightLevel = 0;
    size_t eventIdx = 0;

    std::cout << std::endl << "Результат поиска:" << std::endl;
    SetConsoleColor(DEFAULT_COLOR);

    for (size_t i = 0; i < text.size(); ++i)
    {
        while (eventIdx < events.size() && events[eventIdx].first == i)
        {
            if (events[eventIdx].second)
            {
                if (highlightLevel++ == 0)
                    SetConsoleColor(FOREGROUND_RED);
            }
            else
                if (--highlightLevel == 0)
                    SetConsoleColor(DEFAULT_COLOR);
            ++eventIdx;
        }
        std::cout << text[i];
    }

    if (highlightLevel > 0)
        SetConsoleColor(DEFAULT_COLOR);
}

void ToLower(std::string& text)
{
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c)
        {
            return std::tolower(c);
        });
}

void main() {
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
