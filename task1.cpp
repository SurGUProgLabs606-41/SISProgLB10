// S - нетерминалом. Переменная, которую нужно проверить на соответствие грамматике S -> aSb | ab
#include <iostream>
#include <string>

using namespace std;

// Проверка соответствия строки грамматике S -> aSb | ab
bool parse_S(const string& input, size_t& pos)
{
    // Правило S -> ab. Простая проверка
    if (pos + 1 < input.size() &&
        input[pos] == 'a' && input[pos + 1] == 'b')
    {
        pos += 2;
        return true;
    }

    // Правило S -> aSb. Проверка длинных строк
    if (pos < input.size() && input[pos] == 'a')
    {
        pos++;

        if (parse_S(input, pos)) // Рекурсивный вызов для проверки следующей части строки
        {
            if (pos < input.size() && input[pos] == 'b')
            {
                pos++;
                return true;
            }
        }
    }

    return false;
}

int main()
{
    string input;

    cout << "Введите строку: ";
    cin >> input;

    size_t pos = 0;
    // При aabbab доходит до aabb и pos = 4, а size = 6. Поэтому выводиться 0.
    if (parse_S(input, pos) && pos == input.size()) // Проверка, что вся строка была обработана
    {
        cout << "Да" << endl;
    }
    else
    {
        cout << "Нет" << endl;
    }

    return 0;
}
