// Левую рекурсию убрали, потому что при использовании метода рекурсивного спуска она приводит к бесконечным вызовам функции. 
// Чтобы этого избежать, мы переписали грамматику так, чтобы сначала обрабатывался основной элемент выражения, а затем дополнительные операции. 
// Благодаря этому парсер может корректно разбирать арифметические выражения
#include <iostream>
#include <string>

using namespace std;

string input; // Строка пользователя
size_t pos = 0; // Позиция в строке

// Объявления функций
bool parse_E();
bool parse_E_prime();
bool parse_T();
bool parse_T_prime();
bool parse_F();

// E -> T E'
bool parse_E()
{
    if (!parse_T())
        return false;

    return parse_E_prime();
}

// E' -> + T E' | - T E' | epsilon
bool parse_E_prime()
{
    if (pos < input.size() &&
        (input[pos] == '+' || input[pos] == '-'))
    {
        pos++;

        if (!parse_T())
            return false;

        return parse_E_prime();
    }

    return true;
}

// T -> F T'
bool parse_T()
{
    if (!parse_F())
        return false;

    return parse_T_prime();
}

// T' -> * F T' | / F T' | epsilon
bool parse_T_prime()
{
    if (pos < input.size() &&
        (input[pos] == '*' || input[pos] == '/'))
    {
        pos++;

        if (!parse_F())
            return false;

        return parse_T_prime();
    }

    return true;
}

// F -> (E) | число
bool parse_F()
{
    // Проверка скобок
    if (pos < input.size() && input[pos] == '(')
    {
        pos++;

        if (!parse_E())
            return false;

        if (pos < input.size() && input[pos] == ')')
        {
            pos++;
            return true;
        }

        return false;
    }

    // Проверка числа
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

    return false;
}

int main()
{
    cout << "Введите выражение: ";
    cin >> input;

    pos = 0;

    if (parse_E() && pos == input.size())
    {
        cout << "Да" << endl;
    }
    else
    {
        cout << "Нет" << endl;
    }

    return 0;
}
