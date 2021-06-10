#include <Windows.h>
#include <iostream>
#include <deque>
#include <iterator>
#include <string>
#include <stack>
#include <cmath>

using namespace std;

void func(double num, double eps, int& ch, int& zn)
{
    int a = 1; int b = 1;
    int mn = 2; // множитель для начального приближения
    int iter = 0;
    ch = a; zn = b;
    // Поиск начального приближения
    double c = 1;
    do {
        b++;
        c = (double)a / b;
    } while ((num - c) < 0);
    if ((num - c) < eps)
    {
        ch = a; zn = b;
        return;
    }
    b--;
    c = (double)a / b;
    if ((num - c) > -eps)
    {
        ch = a; zn = b;
        return;
    }
    // Уточнение
    while (iter < 20000)
    {
        int cc = a * mn, zz = b * mn;
        iter++;
        do {
            zz++;
            c = (double)cc / zz;
        } while ((num - c) < 0);
        if ((num - c) < eps)
        {
            ch = cc; zn = zz;
            return;
        }
        zz--;
        c = (double)cc / zz;
        if ((num - c) > -eps)
        {
            ch = cc; zn = zz;
            return;
        }
        mn++;
    }
}

bool IsOperand(char symbol)
{
    if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/' || symbol == '(' || symbol == ')'
        || symbol == '^')
        return true;
    return false;
}

string getOPZ(string viraj, deque<char> &d1)
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
    for (unsigned int i = d1.size() - 1; i > 0; i--)
    {
        OPZ += d1[i];
    }
    return OPZ;
}

double cuberoot(double a, double zn, double b)
{
    if ((int)zn % 2 == 0 && a < 0) //если знаменатель чётный и число отрицательное
    {
        return INFINITY;
    }
    else if (a < 0 && int(zn) % 2 != 0) // если число отрицательное и знаменатель нечётный
    {
        return -pow(-a, b);
    }
    else if (a > 0) // если число положительное то без разницы какой будет знаменатель
    {
        return pow(a, b);
    }
    return INFINITY;
}

float calculate(string stroka, stack<float> &stk)
{
    float v1, v2, res;
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
            v2 = stk.top();
            stk.pop();
            v1 = stk.top();
            stk.pop();
            switch (stroka[i])
            {
            case '+': res = v1 + v2;
                break;
            case '-': res = v1 - v2;
                break;
            case '*': res = v1 * v2;
                break;
            case '/': res = v1 / v2;
                break;
            case '^': 
                if (v2 >= 1)
                {
                    res = pow(v1, v2);
                }
                else if (v2 < 0)
                {
                    res = pow(v1, v2);
                }
                else
                {
                    int ch, zn;
                    func(v2, eps, ch, zn); // функция нахождения знаменателя обыкновенной дроби
                    res = cuberoot(v1, zn, v2);
                }
                break;
            default:
                break;
            }
            stk.push(res);
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

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string ishod_virajenie, temp_str;
    deque<char> d1;
    
    cout << "Введите выражение: "; // отрицательные степени
    getline(cin, ishod_virajenie);
    temp_str = ishod_virajenie;    // посчитать сначала исключение со степенью, потом все выражение 
    int pos = 0;
    while (temp_str.find('(', pos) != -1) // вставляем нули перед минусами для корректного подсчёта
    {
        if (temp_str[temp_str.find('(', pos) + 1] == '-')
        {
            temp_str.insert(temp_str.find('(', pos) + 1, "0");
        }
        pos = temp_str.find('(', pos) + 1;
    }

    string result_opz = getOPZ(temp_str, d1);
    stack<float> stk;

    cout << "\n" << ishod_virajenie << " = ";
    float result = calculate(result_opz, stk);
    if (result == INFINITY) 
    {
        cout << "Value error!" << endl;
    } 
    else 
    {
        cout << result << endl;
    }
}