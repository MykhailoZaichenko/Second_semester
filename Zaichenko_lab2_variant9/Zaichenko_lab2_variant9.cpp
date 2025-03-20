#include <iostream>
#include <cmath>
#include <windows.h>
#include <iomanip>

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

// Ітеративний половинного ділення
double bisection_iterative(double (*f)(double), double a, double b, double eps, int n, int& i) {
    double fa = f(a), fb = f(b);

    if (fa * fb > 0) {
        if (fa > 0 && fb > 0)
            std::cerr << "Error: The chosen interval [" << a << ", " << b << "] does not contain a root, as the function values at both ends are positive." << std::endl;
        else if (fa < 0 && fb < 0)
            std::cerr << "Error: The chosen interval [" << a << ", " << b << "] does not contain a root, as the function values at both ends are negative." << std::endl;
        else
            std::cerr << "Error: The chosen interval [" << a << ", " << b << "] contains multiple roots." << std::endl;
        return NAN;
    }

    double mid;
    i = 0;

    while (true) {
        if (i >= n) {
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
        i++;
    }

    return mid;
}

double bisection_recursive_loop(double (*f)(double), double a, double b, double eps, int n, int& i) {
    double fa = f(a), fb = f(b);
    double mid = (a + b) / 2;
    double fmid = f(mid);

    if (i >= n) {
        return mid;
    }

    if (fabs(fmid) < eps) {
        return mid;
    }

    i++;

    if (f(a) * fmid < 0) {
        return bisection_recursive_loop(f, a, mid, eps, n, i);
    }
    else {
        return bisection_recursive_loop(f, mid, b, eps, n, i);
    }
}

// Рекурсивний половинного ділення
double bisection_recursive(double (*f)(double), double a, double b, double eps, int n, int& i) {
    double fa = f(a), fb = f(b);

    if (fa * fb > 0) {
        if (fa > 0 && fb > 0)
            std::cerr << "Error: The chosen interval [" << a << ", " << b << "] does not contain a root, as the function values at both ends are positive." << std::endl;
        else if (fa < 0 && fb < 0)
            std::cerr << "Error: The chosen interval [" << a << ", " << b << "] does not contain a root, as the function values at both ends are negative." << std::endl;
        else
            std::cerr << "Error: The chosen interval [" << a << ", " << b << "] contains multiple roots." << std::endl;
        return NAN;
    }
	return bisection_recursive_loop(f, a, b, eps, n, i);
}

int main() {

    std::cout << "Bisection Method\n";
    std::cout << "1. f(x) = 2x - 9\n";
    std::cout << "2. f(x) = -2x^3 + 1\n";
    std::cout << "3. f(x) = 0.75x * sin(pi * x / 4) + 0.9 * cos(pi * x / 4)\n";
    std::cout << "Choose a function: ";

    int choice;
    while (!(std::cin >> choice) || choice < 1 || choice > 3) {
        std::cout << "Error: Invalid function number. Enter function number (1-3): ";
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

    std::cout << "Enter interval bounds [a, b]: ";
    while (!(std::cin >> a >> b) || a >= b) {
        std::cout << "Error: a must be less than b. Enter interval bounds [a, b]: ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    std::cout << "Enter precision (eps): ";
    while (!(std::cin >> eps) || eps <= 0) {
        std::cout << "Error: Precision must be a positive number. Enter precision (eps): ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    std::cout << "Enter maximum number of iterations: ";
    while (!(std::cin >> n) || n <= 0) {
        std::cout << "Error: Number of iterations must be a positive number. Enter maximum number of iterations: ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }

    int iterations = 0;

    double root_iter = bisection_iterative(f, a, b, eps, n, iterations);
    if (iterations >= n) {
        std::cout << "\nWarning: Maximum number of iterations reached." << std::endl;
    }
    std::cout << "Iterative method:" << std::endl;
    std::cout << "Root found: " << std::fixed << std::setprecision(8) << root_iter << std::endl;
    std::cout << "Number of iterations: " << iterations << std::endl;
    std::cout << "f(root) = " << f(root_iter) << std::endl;

	std::cout << std::endl;

    iterations = 0;
    double root_rec = bisection_recursive(f, a, b, eps, n, iterations);
    if (iterations >= n) {
        std::cout << "\nWarning: Maximum number of iterations reached." << std::endl;
    }
    std::cout << "Recursive method:" << std::endl;
    std::cout << "Root found: " << std::fixed << std::setprecision(8) << root_rec << std::endl;
    std::cout << "Number of iterations: " << iterations << std::endl;
    std::cout << "f(root) = " << f(root_rec) << std::endl;

    return 0;
}

// Tests:
//
// test1
// input: f(x)=2x-9; [0, 10]; eps = 1e-3; n = 100
// Iterative output: 
// Root found: 4.49951172 
// Number of iterations: 11
// f(root) = -0.00097656
//
// Recursive output:
// Root found: 4.49951172
// Number of iterations : 11
// f(root) = -0.00097656
//
// test2
//
// input: f(x)=2x-9; [0, 10]; eps = 1e-5; n = 100
// Iterative output: 
// Root found : 4.49999809
// Number of iterations : 19
// f(root) = -0.00000381
//
// Recursive output:
// Root found : 4.49999809
// Number of iterations : 19
// f(root) = -0.00000381
//
// test3
//
// // input: f(x)=2x-9; [0, 10]; eps = 1e-3; n = 3
// Iterative output:
// // Warning: Maximum number of iterations reached.
// Root found : 3.75000000
// Number of iterations : 3
// f(root) = -1.50000000
//
// Recursive output:
// // Warning: Maximum number of iterations reached.
// Root found: 4.37500000
// Number of iterations: 3
// f(root) = -0.25000000
// 
// test4
//
// input: f(x)=-2x^3+1; [-1, 1]; eps = 1e-3; n = 100
// Iterative output: 
// Root found : 0.79394531
// Number of iterations : 10
// f(root) = -0.00092552
//
// Recursive output:
// Root found : 0.79394531
// Number of iterations : 10
// f(root) = -0.00092552
//
// test5
//
// input: f(x)=-2x^3+1; [-1, 1]; eps = 1e-5; n = 100
// Iterative output: 
// Root found : 0.79370117
// Number of iterations : 12
// f(root) = -0.00000244
//
// Recursive output:
// Root found : 0.79370117
// Number of iterations : 12
// f(root) = -0.00000244
//
// test6
//
// input: f(x)=-2x^3+1; [-1, 1]; eps = 1e-3; n = 3
// Iterative output: 
// Warning: Maximum number of iterations reached.
// Root found : 0.75000000
// Number of iterations : 3
// f(root) = 0.15625000
//
// Recursive output:
// Warning : Maximum number of iterations reached.
// Root found : 0.87500000
// Number of iterations : 3
// f(root) = -0.33984375
//
// test7
// input: f(x)=0.75x*sin(pi*x/4)+0.9*cos(pi*x/4); [-4, 3]; eps = 1e-3; n = 100
// Iterative output: 
// Root found : -3.58898926
// Number of iterations : 12
// f(root) = 0.00038723
//
// Recursive output:
// Root found : -3.58898926
// Number of iterations : 12
// f(root) = 0.00038723
//
// test8
// input: f(x)=0.75x*sin(pi*x/4)+0.9*cos(pi*x/4); [-4, 3]; eps = 1e-5; n = 100
// Iterative output: 
// Root found : -3.58918285
// Number of iterations : 19
// f(root) = 0.00000172
//
// Recursive output:
// Root found : -3.58918285
// Number of iterations : 19
// f(root) = 0.00000172
//
// test9
// input: f(x)=0.75x*sin(pi*x/4)+0.9*cos(pi*x/4); [-4, 3]; eps = 1e-3; n = 3
// Iterative output:
// Warning: Maximum number of iterations reached.
// Root found : -3.12500000
// Number of iterations : 3
// f(root) = 0.79114985
//
// Recursive output:
// Warning: Maximum number of iterations reached.
// Root found : -3.56250000
// Number of iterations : 3
// f(root) = 0.05273792