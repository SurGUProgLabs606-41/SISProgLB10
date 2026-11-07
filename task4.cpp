#include <iostream>
#include <string>

using namespace std;

string input; // Строка пользователя
size_t pos = 0; // Позиция в строке

bool errorFound = false; // Флаг, указывающий, была ли найдена ошибка
string expected; // Ожидаемый символ или правило, которое не было найдено

// Пропуск пробелов
void skipSpaces()
{
    while (pos < input.size() && input[pos] == ' ')
    {
        pos++;
    }
}

// Запись первой найденной ошибки
void setError(const string& message)
{
    if (!errorFound)
    {
        errorFound = true;
        expected = message;
    }
}

// Объявления функций
bool parse_E();
bool parse_T();
bool parse_F();

// E -> T E'
bool parse_E()
{
    if (!parse_T())
        return false;

    // E' -> + T E' | - T E' | epsilon
    while (true)
    {
        skipSpaces();

        if (pos < input.size() &&
            (input[pos] == '+' || input[pos] == '-'))
        {
            pos++;

            if (!parse_T())
                return false;
        }
        else
        {
            return true;
        }
    }
}

// T -> F T'
bool parse_T()
{
    if (!parse_F())
        return false;

    // T' -> * F T' | / F T' | epsilon
    while (true)
    {
        skipSpaces();

        if (pos < input.size() &&
            (input[pos] == '*' || input[pos] == '/'))
        {
            pos++;

            if (!parse_F())
                return false;
        }
        else
        {
            return true;
        }
    }
}

// F -> (E) | число
bool parse_F()
{
    skipSpaces();

    // Обработка скобок
    if (pos < input.size() && input[pos] == '(')
    {
        pos++;

        if (!parse_E())
            return false;

        skipSpaces();

        if (pos < input.size() && input[pos] == ')')
        {
            pos++;
            return true;
        }

        setError("')'");
        return false;
    }

    // Обработка числа
    if (pos < input.size() &&
        input[pos] >= '0' && input[pos] <= '9')
    {
        while (pos < input.size() &&
               input[pos] >= '0' && input[pos] <= '9')
        {
            pos++;
        }

        return true;
    }

    setError("число или '('");
    return false;
}

int main()
{
    cout << "Введите выражение: ";
    getline(cin, input);

    pos = 0;
    errorFound = false;

    bool result = parse_E();

    skipSpaces();

    // Проверка, что разобрана вся строка
    if (result && pos < input.size())
    {
        setError("конец выражения");
    }

    if (result && pos == input.size())
    {
        cout << "Да" << endl;
    }
    else
    {
        cout << "\nОшибка в позиции " << pos << ":" << endl;
        cout << "Ожидалось: " << expected << endl;

        if (pos >= input.size())
        {
            cout << "Получено: конец строки" << endl;
        }
        else
        {
            cout << "Получено: '" << input[pos] << "'" << endl;
        }
    }

    return 0;
}
