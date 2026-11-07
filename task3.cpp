// Дерево разбора — это структура, которая показывает последовательность применения грамматических правил
// E  -> T E'
// E' -> + T E' | - T E' | ε
// T  -> F T'
// T' -> * F T' | / F T' | ε
// F  -> (E) | число
#include <iostream>
#include <string>

using namespace std;

// Узел дерева разбора
struct Node
{
    string value;
    Node* firstChild;
    Node* nextSibling;

    Node(string val)
    {
        value = val;
        firstChild = nullptr;
        nextSibling = nullptr;
    }
};

string input;
size_t pos = 0;

// Объявления функций
Node* parse_E();
Node* parse_E_prime();
Node* parse_T();
Node* parse_T_prime();
Node* parse_F();

// Добавление дочернего узла
// Добавляет новый дочерний узел к существующему узлу. Например, для E она добавляет T и E'.
// Она проверяет входные символы, применяет нужное правило и создаёт соответствующие узлы дерева.
void addChild(Node* parent, Node* child)
{
    if (parent == nullptr || child == nullptr)
        return;

    if (parent->firstChild == nullptr)
    {
        parent->firstChild = child;
    }
    else
    {
        Node* current = parent->firstChild;

        while (current->nextSibling != nullptr)
            current = current->nextSibling;

        current->nextSibling = child;
    }
}

// E -> T E'
Node* parse_E()
{
    Node* node = new Node("E");

    Node* t = parse_T();

    if (t == nullptr)
    {
        delete node;
        return nullptr;
    }

    addChild(node, t);

    Node* e = parse_E_prime();

    if (e == nullptr)
    {
        // Освобождение дерева выполняется в main
        return node;
    }

    addChild(node, e);

    return node;
}

// E' -> + T E' | - T E' | ε
Node* parse_E_prime()
{
    Node* node = new Node("E'");

    if (pos < input.size() &&
        (input[pos] == '+' || input[pos] == '-'))
    {
        char operation = input[pos++];

        addChild(node, new Node(string(1, operation)));

        Node* t = parse_T();

        if (t == nullptr)
            return node;

        addChild(node, t);

        Node* e = parse_E_prime();

        if (e != nullptr)
            addChild(node, e);
    }
    else
    {
        addChild(node, new Node("ε"));
    }

    return node;
}

// T -> F T'
Node* parse_T()
{
    Node* node = new Node("T");

    Node* f = parse_F();

    if (f == nullptr)
    {
        delete node;
        return nullptr;
    }

    addChild(node, f);

    Node* t = parse_T_prime();

    if (t != nullptr)
        addChild(node, t);

    return node;
}

// T' -> * F T' | / F T' | ε
Node* parse_T_prime()
{
    Node* node = new Node("T'");

    if (pos < input.size() &&
        (input[pos] == '*' || input[pos] == '/'))
    {
        char operation = input[pos++];

        addChild(node, new Node(string(1, operation)));

        Node* f = parse_F();

        if (f == nullptr)
            return node;

        addChild(node, f);

        Node* t = parse_T_prime();

        if (t != nullptr)
            addChild(node, t);
    }
    else
    {
        addChild(node, new Node("ε"));
    }

    return node;
}

// F -> (E) | число
Node* parse_F()
{
    Node* node = new Node("F");

    // Обработка скобок
    if (pos < input.size() && input[pos] == '(')
    {
        pos++;

        addChild(node, new Node("("));

        Node* e = parse_E();

        if (e == nullptr)
        {
            delete node;
            return nullptr;
        }

        addChild(node, e);

        if (pos < input.size() && input[pos] == ')')
        {
            pos++;
            addChild(node, new Node(")"));
            return node;
        }

        delete node;
        return nullptr;
    }

    // Обработка числа
    if (pos < input.size() &&
        input[pos] >= '0' && input[pos] <= '9')
    {
        string number;

        while (pos < input.size() &&
               input[pos] >= '0' && input[pos] <= '9')
        {
            number += input[pos];
            pos++;
        }

        addChild(node, new Node(number));
        return node;
    }

    delete node;
    return nullptr;
}

// Вывод дерева разбора
// Обходит уже построенное дерево и выводит его на экран с отступами и ветвями, показывая взаимосвязи между узлами.
void printTree(Node* node, string prefix = "", bool isLast = true)
{
    if (node == nullptr)
        return;

    cout << prefix;

    if (prefix != "")
        cout << (isLast ? "└── " : "├── ");

    cout << node->value << endl;

    Node* child = node->firstChild;

    while (child != nullptr)
    {
        bool last = (child->nextSibling == nullptr);

        string newPrefix = prefix;

        if (node->value != "")
        {
            if (prefix == "")
                newPrefix = "";
        }

        if (prefix != "")
            newPrefix += (isLast ? "    " : "│   ");

        else
            newPrefix = "    ";

        printTree(child, newPrefix, last);

        child = child->nextSibling;
    }
}

// Освобождение памяти
// Освобождает память, занятую всеми узлами дерева, после завершения работы программы.
void deleteTree(Node* node)
{
    if (node == nullptr)
        return;

    Node* child = node->firstChild;

    while (child != nullptr)
    {
        Node* next = child->nextSibling;
        deleteTree(child);
        child = next;
    }

    delete node;
}

int main()
{
    cout << "Введите выражение: ";
    cin >> input;

    pos = 0;

    Node* root = parse_E();

    if (root != nullptr && pos == input.size())
    {
        cout << "\nДерево разбора:\n";
        printTree(root);
    }
    else
    {
        cout << "Нет: выражение некорректно" << endl;
    }

    deleteTree(root);

    return 0;
}

// Программа получает арифметическое выражение и начинает разбор с символа E. 
// Для каждого нетерминала предусмотрена отдельная функция, которая создаёт соответствующие узлы дерева.
// Нетерминал - обозначение в грамматике, которое можно заменить другими символами или по правилам.
