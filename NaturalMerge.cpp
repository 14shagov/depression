#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

void NaturalMergeSort(vector<int>& arr)
{
    // Базовый случай: массив из 0-1 элементов уже отсортирован
    if(arr.size() <= 1) return;

    vector<int> temp(arr.size());
    bool sorted = false;

    // Главный цикл сортировки: повторяем проходы пока массив не отсортирован
    while(!sorted)
    {
        sorted = true;
        size_t start = 0;
        size_t temp_index = 0;

        // Проход по массиву для поиска и слияния серий
        while(start < arr.size())
        {
            // Этап 1: Поиск первой естественной серии (возрастающей последовательности)
            size_t end1 = start;
            while(end1 < arr.size()-1 && arr[end1] <= arr[end1+1])
                end1++;

            // Проверка полной отсортированности после первого прохода
            if(start == 0 && end1 == arr.size()-1) return;

            // Этап 2: Поиск второй серии для слияния
            size_t start2 = end1+1;
            size_t end2 = start2;
            if(start2 < arr.size())
            {
                while(end2 < arr.size()-1 && arr[end2] <= arr[end2+1])
                    end2++;
            }
            else
                end2 = arr.size()-1;

            // Этап 3: Слияние двух найденных серий
            size_t i = start;
            size_t j = start2;
            
            // Слияние элементов пока обе серии не закончатся
            while(i <= end1 && j <= end2)
            {
                temp[temp_index++] = arr[i] <= arr[j] 
                    ? arr[i++] 
                    : arr[j++];
            }

            // Докопирование остатков из первой серии
            while(i <= end1) temp[temp_index++] = arr[i++];
            
            // Докопирование остатков из второй серии
            while(j <= end2) temp[temp_index++] = arr[j++];

            // Если нашлись две серии - продолжаем процесс
            if(start2 <= end2) sorted = false;
            
            // Переход к следующей паре серий
            start = end2+1;
        }

        // Замена исходного массива временным буфером
        swap(arr, temp);
        temp.clear();
        temp.resize(arr.size());
    }
}

int main()
{
    srand(time(nullptr));

    // Генерация входных данных
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);
    generate(arr.begin(), arr.end(), [](){ return rand()%100; });

    // Вывод исходного массива
    cout << "\nOriginal array:\n";
    for(const auto& num : arr)
        cout << num << " ";

    // Основной процесс сортировки
    NaturalMergeSort(arr);

    // Вывод результата
    cout << "\n\nSorted array:\n";
    for(const auto& num : arr)
        cout << num << " ";

    return 0;
}

/*
Как работает Natural Merge Sort:
[Этапы алгоритма]            [Соответствующий код]
---------------------------------------------------------------
1. Инициализация:            Создание временного буфера (temp),
   подготовка временных      флага sorted, начальных индексов
   структур данных           (start, temp_index)

2. Обнаружение серий:        Вложенные while-циклы для поиска
   Поиск максимальных        end1 и end2 - границ упорядоченных
   упорядоченных участков    участков (серий)

3. Слияние серий:            Двойной цикл слияния с выбором
   Попарное объединение      минимального элемента и докопированием
   соседних последова-       остатков через тернарный оператор
   тельностей

4. Проверка состояния:       Флаг sorted и проверка
   Определение необходимости (start == 0 && end1 == arr.size()-1)
   продолжения сортировки

5. Обновление данных:        swap(arr, temp) для замены исходного
   Замена исходного          массива новым объединенным вариантом
   массива временным
   буфером

6. Повторение процесса:      Главный while-цикл, продолжающий
   Многоходовая сортировка   работу пока sorted == false
   до полного упорядочивания
*/
