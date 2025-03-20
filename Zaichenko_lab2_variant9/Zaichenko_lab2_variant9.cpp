#include <iostream>
#include <cmath>
#include <windows.h>
#include <iomanip>



// Функції для обчислення значень f(x)
double f1(double x) {
    return 2 * x - 9;
}

double f2(double x) {
    return -2 * pow(x, 3) + 1;
}

double f3(double x) {
    const double PI = 3.14159265358979323846;
    return 0.75 * x * sin(PI * x / 4) + 0.9 * cos(PI * x / 4);
}

// Ітеративний метод половинного ділення
double bisection_iterative(double (*f)(double), double a, double b, double eps, int n, int& iterations) {
    double fa = f(a), fb = f(b);

    if (fa * fb > 0) {
        if (fa > 0 && fb > 0)
            std::cerr << "Помилка: Обраний проміжок [" << a << ", " << b << "] не містить кореня, оскільки значення функції в обох кінцях додатні." <<std::endl;
        else if (fa < 0 && fb < 0)
            std::cerr << "Помилка: Обраний проміжок [" << a << ", " << b << "] не містить кореня, оскільки значення функції в обох кінцях від'ємні." <<std::endl;
        else
            std::cerr << "Помилка: Обраний проміжок [" << a << ", " << b << "] містить декілька коренів." <<std::endl;
        return NAN;
    }

    double mid;
    iterations = 0;

    while ((b - a) / 2 > eps) {
        if (iterations >= n) {
            std::cout << "\nПопередження: Досягнуто максимальної кількості ітерацій." << std::endl;
            break;
        }

        mid = (a + b) / 2;
        double fmid = f(mid);

        if (fabs(fmid) < eps) {
            return mid;
        }

        if (fa * fmid < 0) {
            b = mid;
            fb = fmid;
        }
        else {
            a = mid;
            fa = fmid;
        }
        iterations++;
    }

    return mid;
}

// Рекурсивний метод половинного ділення
double bisection_recursive(double (*f)(double), double a, double b, double eps, int n, int& iterations) {
    if (iterations >= n) {
        std::cout << "\nПопередження: Досягнуто максимальної кількості ітерацій." << std::endl;
        return (a + b) / 2;
    }

    if (iterations == 0) {
        double fa = f(a), fb = f(b);

        if (fa * fb > 0) {
            if (fa > 0 && fb > 0)
                std::cerr << "Помилка: Обраний проміжок [" << a << ", " << b << "] не містить кореня, оскільки значення функції в обох кінцях додатні." <<std::endl;
            else if (fa < 0 && fb < 0)
                std::cerr << "Помилка: Обраний проміжок [" << a << ", " << b << "] не містить кореня, оскільки значення функції в обох кінцях від'ємні." <<std::endl;
            else
                std::cerr << "Помилка: Обраний проміжок [" << a << ", " << b << "] містить декілька коренів.\n";
            return NAN;
        }
    }

    double mid = (a + b) / 2;
    double fmid = f(mid);

    if (fabs(fmid) < eps || (b - a) / 2 < eps || iterations >= n) {
        return mid;
    }

    iterations++;

    if (f(a) * fmid < 0) {
        return bisection_recursive(f, a, mid, eps, n, iterations);
    }
    else {
        return bisection_recursive(f, mid, b, eps, n, iterations);
    }
}

int main() {
    SetConsoleCP(1251); // встановлення кодування Windows-1251 в  потік введення
    SetConsoleOutputCP(1251); // встановлення кодування Windows-1251 в  потік виведення

    std::cout << "Метод половинного ділення\n";
    std::cout << "1. f(x) = 2x - 9\n";
    std::cout << "2. f(x) = -2x^3 + 1\n";
    std::cout << "3. f(x) = 0.75x * sin(pi * x / 4) + 0.9 * cos(pi * x / 4)\n";
    std::cout << "Оберіть функцію: ";

    int choice;
    while (!(std::cin >> choice) || choice < 1 || choice > 3) {
        std::cout << "Помилка: введено некоректний номер функції. Введи номер фунції (1-3): ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    double (*f)(double) = nullptr;
    switch (choice) {
    case 1: f = f1; break;
    case 2: f = f2; break;
    case 3: f = f3; break;
    default:
       return 1;
    }

    double a, b, eps;
    int n;

    std::cout << "Введіть межі проміжку [a, b]: ";
	while (!(std::cin >> a >> b) || a >= b) {
		std::cout << "Помилка: a має бути менше за b. Введіть межі проміжку [a, b]: ";
		std::cin.clear();
		std::cin.ignore(10000, '\n');
	}

    std::cout << "Введіть точність (eps): ";
	while (!(std::cin >> eps) || eps <= 0) {
		std::cout << "Помилка: точність має бути додатним числом. Введіть точність (eps): ";
		std::cin.clear();
		std::cin.ignore(10000, '\n');
	}

    std::cout << "Введіть максимальну кількість ітерацій: ";
	while (!(std::cin >> n) || n <= 0) {
		std::cout << "Помилка: кількість ітерацій має бути додатним числом. Введіть максимальну кількість ітерацій: ";
		std::cin.clear();
		std::cin.ignore(10000, '\n');
	}

    int iterations = 0;

    // Виконання ітеративного методу
    double root_iter = bisection_iterative(f, a, b, eps, n, iterations);
    std::cout << "Ітеративний метод:" << std::endl;
    std::cout << "Знайдений корінь: " << std::fixed << std::setprecision(8) << root_iter <<  std::endl;
    std::cout << "Кількість ітерацій: " << iterations <<  std::endl;
    std::cout << "f(root) = " << f(root_iter) <<  std::endl;

    // Виконання рекурсивного методу
    iterations = 0;
    double root_rec = bisection_recursive(f, a, b, eps, n, iterations);
    std::cout << "Рекурсивний метод:" << std::endl;
    std::cout << "Знайдений корінь: " << std::fixed << std::setprecision(8) << root_rec <<  std::endl;
    std::cout << "Кількість ітерацій: " << iterations <<  std::endl;
    std::cout << "f(root) = " << f(root_rec) << std::endl;

    return 0;
}

//Тести:
//
//test1
//
//Iterative
//input: f(x)=2x-9; [0, 10]; eps = 1e-3; n = 100
//output: root = 4.50000000; f(root) = 0.00000000; i = 11
//
//Recursive
//input: f(x)=2x-9; [0, 10]; eps = 1e-3; n = 100
//output: root = 4.50000000; f(root) = 0.00000000; i = 11
//
//test2
//
//Iterative
//input: f(x)=2x-9; [0, 10]; eps = 1e-5; n = 100
//output: root = 4.50000000; f(root) = 0.00000000; i = 19
//
//Recursive
//input: f(x)=2x-9; [0, 10]; eps = 1e-5; n = 100
//output: root = 4.50000000; f(root) = 0.00000000; i = 19
//
//test3 (maximum number of the steps)
//
//Iterative
//input: f(x)=2x-9; [0, 10]; eps = 1e-3; n = 3
//output: root = 4.37500000; f(root) = -0.25000000; i = 3
//
//Recursive
//input: f(x)=2x-9; [0, 10]; eps = 1e-3; n = 3
//output: root = 4.37500000; f(root) = -0.25000000; i = 3
//
//test4
//
//Iterative
//input: f(x)=-2x^3+1; [-1, 1]; eps = 1e-3; n = 100
//output: root = 0.79370117; f(root) = -0.00001526; i = 11
//
//Recursive
//input: f(x)=-2x^3+1; [-1, 1]; eps = 1e-3; n = 100
//output: root = 0.79370117; f(root) = -0.00001526; i = 11
//
//test5
//
//Iterative
//input: f(x)=-2x^3+1; [-1, 1]; eps = 1e-5; n = 100
//output: root = 0.79370053; f(root) = -0.00000000; i = 19
//
//Recursive
//input: f(x)=-2x^3+1; [-1, 1]; eps = 1e-5; n = 100
//output: root = 0.79370053; f(root) = -0.00000000; i = 19
//
//test6 (maximum number of the steps)
//
//Iterative
//input: f(x)=-2x^3+1; [-1, 1]; eps = 1e-3; n = 3
//output: root = 0.0; f(root) = 1.0; i = 3
//
//Recursive
//input: f(x)=-2x^3+1; [-1, 1]; eps = 1e-3; n = 3
//output: root = 0.0; f(root) = 1.0; i = 3
//
//test7
//
//Iterative
//input: f(x)=0.75x*sin(pi*x/4)+0.9*cos(pi*x/4); [-1, 1]; eps = 1e-3; n = 100
//output: root = -0.56835938; f(root) = 0.00000000; i = 11
//
//Recursive
//input: f(x)=0.75x*sin(pi*x/4)+0.9*cos(pi*x/4); [-1, 1]; eps = 1e-3; n = 100
//output: root = -0.56835938; f(root) = 0.00000000; i = 11
//
//test8
//
//Iterative
//input: f(x)=0.75x*sin(pi*x/4)+0.9*cos(pi*x/4); [-1, 1]; eps = 1e-5; n = 100
//output: root = -0.56835938; f(root) = 0.00000000; i = 19
//
//Recursive
//input: f(x)=0.75x*sin(pi*x/4)+0.9*cos(pi*x/4); [-1, 1]; eps = 1e-5; n = 100
//output: root = -0.56835938; f(root) = 0.00000000; i = 19
//
//test9 (maximum number of the steps)
//
//Iterative
//input: f(x)=0.75x*sin(pi*x/4)+0.9*cos(pi*x/4); [-1, 1]; eps = 1e-3; n = 3
//output: root = -0.75; f(root) = 0.09659258; i = 3
//
//Recursive
//input: f(x)=0.75x*sin(pi*x/4)+0.9*cos(pi*x/4); [-1, 1]; eps = 1e-3; n = 3
//output: root = -0.75; f(root) = 0.09659258; i = 3
