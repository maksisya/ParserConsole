#include <Windows.h>
#include <iostream>
#include <deque>
#include <iterator>
#include <string>
#include <stack>
#include <cmath>

using namespace std;

/*!
* \brief Функция для перевода десятичного числа в обыкновенную дробь
* 
* \warning Предполагается, введённая десятичная дробь не имеет целой части
* 
* \param[in] number Десятичное число
* \param[in] eps Вспомогательное значение для дальнейшего уточнения обыкновенной дроби
* 
* \param[out] numerator Числитель полученной обыкновенной дроби
* \param[out] denominator Знаменатель полученной обыкновенной дроби
* 
* В данной функции реализован алгоритм перевода десятичного числа в обыкновенную 
* дробь.
*/

void Decimal_To_Fraction(double number, double eps, int& numerator, int& denominator)
{
    int temp_numerator = 1; int temp_denominator = 1; // начальный числитель и знаменатель
    int factor = 2; // множитель для начального приближения
    int iter = 0; // итератор
    numerator = temp_numerator; denominator = temp_denominator;
    // Поиск начального приближения
    double first_approximation = 1; // приближение
    do {
        temp_denominator++;
        first_approximation = (double)temp_numerator / temp_denominator;
    } while ((number - first_approximation) < 0);
    if ((number - first_approximation) < eps)
    {
        numerator = temp_numerator; denominator = temp_denominator;
        return;
    }
    temp_denominator--;
    first_approximation = (double)temp_numerator / temp_denominator;
    if ((number - first_approximation) > -eps)
    {
        numerator = temp_numerator; denominator = temp_denominator;
        return;
    }
    // Уточнение
    while (iter < 20000)
    {
        int second_approximation = temp_numerator * factor, refined_denominator = temp_denominator * factor;
        iter++;
        do {
            refined_denominator++;
            first_approximation = (double)second_approximation / refined_denominator;
        } while ((number - first_approximation) < 0);
        if ((number - first_approximation) < eps)
        {
            numerator = second_approximation; denominator = refined_denominator;
            return;
        }
        refined_denominator--;
        first_approximation = (double)second_approximation / refined_denominator;
        if ((number - first_approximation) > -eps)
        {
            numerator = second_approximation; denominator = refined_denominator;
            return;
        }
        factor++;
    }
}

/*!
* \brief Функция, определяющая, является ли символ математическим знаком
* 
* \param[in] symbol Исходный символ
* 
* \return Возвращает true, если смвол является знаком математической операции, в остальных случаях - возвращает false
* 
* Данная функция предназначена для определения, является ли символ знаком математической операции, 
* или же является буквой или цифрой.
*/

bool IsOperand(char symbol)
{
    if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/' || symbol == '(' || symbol == ')'
        || symbol == '^')
        return true;
    return false;
}

/*!
* \brief Функция для генерации обратной польской записи из изходного выражения
* 
* \param[in] viraj Исходное выражение, записанное пользователем для преобразования в обратную польскую запись
* \param[in] d1 Deque для хранения знаков математических операций
* 
* \return Возвращает выражение, преобразованное в обратную польскую запись в виде строки
* 
* В данной функции реализован алгоритм преобразования записанного математического выражения
* в обратную польскую запись, для дальнейшего его решения с помощью функции Calculate_OPZ().
*/

string Get_OPZ(string viraj, deque<char> &d1)
{
    string OPZ;
    if (viraj[0] == '-')
    {
        viraj.insert(0, "0");
    }
    for (unsigned int i = 0; i < viraj.length(); )
    {
        if (IsOperand(viraj[i]))
        {
            if (viraj[i] == '(')
            {
                d1.push_back(viraj[i]);
                d1.push_back(' ');
                i++;
                continue;
            }
            else if (viraj[i] == ')')
            {
                if (d1[d1.size() - 2] == '(')
                {
                    d1.pop_back();
                    d1.pop_back();
                    i++;
                    continue;
                }
                deque<char>::iterator it;
                it = d1.end();
                do
                {
                    OPZ += *(it - 2);
                    OPZ += " ";
                    d1.pop_back();
                    d1.pop_back();
                    it = d1.end();
                } while (*(it - 2) != '(');
                d1.pop_back();
                d1.pop_back();
                i++; // исправить этот кусок кода, почему то закрывающая скобка не добавилась в дек
                continue;
            }
            if (d1.size() != 0)
            {
                if (viraj[i] == '*' || viraj[i] == '/')
                {
                    if (d1[d1.size() - 2] != '*' && d1[d1.size() - 2] != '/')
                    {
                        d1.push_back(viraj[i]);
                        d1.push_back(' ');
                        i++;
                    }
                    else
                    {
                        while (d1.size() != 0 && (d1[d1.size() - 2] == '*' || d1[d1.size() - 2] == '/'))
                        {
                            deque<char>::iterator it1;
                            it1 = d1.end();
                            OPZ += *(it1 - 2);
                            OPZ += " ";
                            d1.pop_back();
                            d1.pop_back();
                        }
                        d1.push_back(viraj[i]);
                        d1.push_back(' ');
                        i++;
                    }
                }
                else if (viraj[i] == '^')
                {
                    if (d1[d1.size() - 2] == '^')
                    {
                        deque<char>::iterator it;
                        while (d1.size() != 0 && d1[d1.size() - 2] == '^')
                        {
                            it = d1.end();
                            OPZ += *(it - 2);
                            OPZ += " ";
                            d1.pop_back();
                            d1.pop_back();
                        }
                        d1.push_back(viraj[i]);
                        d1.push_back(' ');
                        i++;
                    }
                    else
                    {
                        d1.push_back(viraj[i]);
                        d1.push_back(' ');
                        i++;
                    }
                }
                else
                {
                    if (d1[d1.size() - 2] == '*' || d1[d1.size() - 2] == '/' || d1[d1.size() - 2] == '+' || d1[d1.size() - 2] == '-' || d1[d1.size() - 2] == '^') // последний символ стека
                    {
                        deque<char>::iterator it;
                        while (d1.size() != 0 && (d1[d1.size() - 2] == '*' || d1[d1.size() - 2] == '/' || d1[d1.size() - 2] == '+' || d1[d1.size() - 2] == '-' || d1[d1.size() - 2] == '^'))
                        {                               // пока символ, находящийся на вершине стека имеет приоритет, больший или равный приоритету текущего символа
                            it = d1.end();
                            OPZ += *(it - 2);
                            OPZ += " ";
                            d1.pop_back();
                            d1.pop_back();
                        }
                        d1.push_back(viraj[i]);
                        d1.push_back(' ');
                        i++;
                    }
                    else
                    {
                        d1.push_back(viraj[i]);
                        d1.push_back(' ');
                        i++;
                    }
                }
            }
            else
            {
                d1.push_back(viraj[i]);
                d1.push_back(' ');
                i++;
            }
        }
        else
        {
            unsigned int j = i;
            while (viraj[j] != '+' && viraj[j] != '-' && viraj[j] != '/' && viraj[j] != '*' && viraj[j] != '('
                && viraj[j] != ')' && viraj[j] != '^')
            {
                OPZ += viraj[j];
                j++;
                if (j >= viraj.length())
                    break;
            }
            if (j >= viraj.length())
                break;
            OPZ += " ";
            i = j;
        }
    }
    d1.push_front(' ');
    if (OPZ[OPZ.length() - 1] == ' ')
    {
        OPZ.erase(OPZ.length() - 1);
    }
    for (int i = d1.size() - 1; i > 0; i--)
    {
        OPZ += d1[i];
    }
    return OPZ;
}

/*!
* \brief Функция для определения знака ответа
* 
* \param[in] value1 Первое число 
* \param[in] denominator Знаменатель обыкновенной дроби
* \param[in] value2 Второе число
* 
* \return Если знаменатель чётный и число под корнем отрицательное - вернет INF. Если число под корнем отрицательное и 
* знаменатель нечётный - вернёт результат возведения в степень, но с минусом. Если под корнем число положительное - вернет
* результат возведения в степень.
* 
* В данной функции реализовано определение знака числа, то есть
* ответ будет либо отрицательным, либо положительным. 
*/

double Extract_Root(double value1, double denominator, double value2)
{
    if ((int)denominator % 2 == 0 && value1 < 0) //если знаменатель чётный и число отрицательное
    {
        return INFINITY;
    }
    else if (value1 < 0 && int(denominator) % 2 != 0) // если число отрицательное и знаменатель нечётный
    {
        return -pow(-value1, value2);
    }
    else if (value1 > 0) // если число положительное то без разницы какой будет знаменатель
    {
        return pow(value1, value2);
    }
    return INFINITY;
}

/*!
* \brief Функция для подсчёта строки исходной польской записи
* 
* \param[in] stroka Строка, преобразованная в обратную польскую запись
* \param[in] stk Вспомогательный stack для хранения знаков операций
* 
* \return Возвращает число, которое является ответом на исходное выражение, введённое пользователем
* 
* Данная функция реализует в себе расшифровку и подсчёт обратной польской записи 
* для получения ответа при решении математического выражения
*/

float Calculate_OPZ(string stroka, stack<float> &stk)
{
    float value1, value2, result;
    double eps = 0.0000001;
    for (int i = 0; i < stroka.length(); i++) 
    {
        int j = i;
        string value;
        while (stroka[j] != ' ' && stroka[j] != '+' && stroka[j] != '-' && stroka[j] != '*'
            && stroka[j] != '/' && stroka[j] != '^')
        {
            value += stroka[j];
            j++;
        }
        if (stk.size() >= 2 && (stroka[j] == '+' || stroka[j] == '-' || stroka[j] == '*'
            || stroka[j] == '/' || stroka[j] == '^'))
        {
            value2 = stk.top();
            stk.pop();
            value1 = stk.top();
            stk.pop();
            switch (stroka[i])
            {
            case '+': result = value1 + value2;
                break;
            case '-': result = value1 - value2;
                break;
            case '*': result = value1 * value2;
                break;
            case '/': result = value1 / value2;
                break;
            case '^': 
                if (value2 >= 1) // если значение степени больше или равно еденице
                {
                    result = pow(value1, value2); // просто возводим в степень
                }
                else if (value2 < 0) // если значение степени меньше нуля
                {
                    result = pow(value1, value2); // просто возводим в степень
                }
                else // в любых других случаях переводим полученое десятичное число в обыкновенную дробь
                {
                    int numerator, denominator; // числитель, знаменатель
                    Decimal_To_Fraction(value2, eps, numerator, denominator); // функция нахождения знаменателя обыкновенной дроби
                    result = Extract_Root(value1, denominator, value2);
                }
                break;
            default:
                break;
            }
            stk.push(result);
            i = j + 1;
            continue;
        }
        if (value.find('.') != -1)
        {
            stk.push(stof(value));
        } 
        else 
        {
            stk.push(atoi(value.c_str()));
        }
        i = j;
    }
    return stk.top();
}

/*!
* \file ParserConsole.cpp
*/

/*!
* \brief Главная функция проекта
*/

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    system("title Parser");

    string original_expression, temp_string;
    deque<char> d1;
    
    cout << "Введите выражение: "; // отрицательные степени
    getline(cin, original_expression);
    temp_string = original_expression;    // посчитать сначала исключение со степенью, потом все выражение 
    int position = 0;
    while (temp_string.find('(', position) != -1) // вставляем нули перед минусами для корректного подсчёта
    {
        if (temp_string[temp_string.find('(', position) + 1] == '-')
        {
            temp_string.insert(temp_string.find('(', position) + 1, "0");
        }
        position = temp_string.find('(', position) + 1;
    }

    string result_opz = Get_OPZ(temp_string, d1);
    stack<float> stk;

    cout << "\n" << original_expression << " = ";
    float result = Calculate_OPZ(result_opz, stk);
    if (result == INFINITY) 
    {
        cout << "Value error!" << endl;
    } 
    else 
    {
        cout << result << endl;
    }
}