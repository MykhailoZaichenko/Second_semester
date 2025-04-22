#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

// Функція для виводу матриці
void print_matrix(int* matrix, int m, int n) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << setw(8) << matrix[i * n + j];
        }
        cout << endl;
    }
}

// Функція для формування зигзагового вектора по діагоналі, починаючи з правого верхнього кута
std::vector<int> zigzag_traversal(int* matrix, int m, int n) {
    std::vector<int> zigzag;
    for (int s = n - 1; s >= -(m - 1); --s) {
        if ((n - 1 - s) % 2 == 0) {
            for (int i = m - 1; i >= 0; --i) {
                int j = s + i;
                if (j >= 0 && j < n) {
                    zigzag.push_back(matrix[i * n + j]);
                }
            }
        }
        else {
            for (int i = 0; i < m; ++i) {
                int j = s + i;
                if (j >= 0 && j < n) {
                    zigzag.push_back(matrix[i * n + j]);
                }
            }
        }
    }
    return zigzag;
}

// Функція для виводу зигзагового вектора
//int main() {
//    int m, n;
//    cout << "Input rows (m > 0): ";
//    while (!(cin >> m) || m <= 0) {
//        cout << "Error: m must be a positive integer." << endl;
//        cout << "Input rows (m > 0): ";
//        cin.clear();
//        cin.ignore(10000, '\n');
//    }
//
//    cout << "Input cols (n > 0): ";
//    while (!(cin >> n) || n <= 0) {
//        cout << "Error: n must be a positive integer." << endl;
//        cout << "Input columns (n > 0): ";
//        cin.clear();
//        cin.ignore(10000, '\n');
//    }
//
//    int* matrix = new int[m * n]; // Неперервна пам’ять (row-major)
//
//    cout << "Enter matrix elements:" << endl;
//    for (int i = 0; i < m; ++i) {
//        for (int j = 0; j < n; ++j) {
//            cout << "matrix[" << i << "][" << j << "] ";
//            cin >> matrix[i * n + j];
//        }
//    }
//
//    cout << "\nInitial matrix:" << endl;
//    print_matrix(matrix, m, n);
//
//    vector<int> zigzag = zigzag_traversal(matrix, m, n);
//
//    cout << endl << "Zigzag vector:" << endl;
//    for (int val : zigzag) {
//        cout << setw(8) << val;
//    }
//    cout << endl;
//
//    delete[] matrix;
//    return 0;
//}

int main() {
    int m = 5, n = 8;
    std::cout << "Input rows (m > 0): " << m << std::endl;
    std::cout << "Input cols (n > 0): " << n << std::endl;

    int* matrix = new int[m * n];
    std::cout << "Enter matrix elements:" << std::endl;

    int values[] = {
        11,12,13,14,15,16,17,18,
        21,22,23,24,25,26,27,28,
        31,32,33,34,35,36,37,38,
        41,42,43,44,45,46,47,48,
        51,52,53,54,55,56,57,58
    };

    int idx = 0;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << "matrix[" << i << "][" << j << "] ";
            matrix[i * n + j] = values[idx++];
            std::cout << matrix[i * n + j] << std::endl;
        }
    }

    std::cout << "\nInitial matrix:" << std::endl;
    print_matrix(matrix, m, n);

    std::vector<int> zigzag = zigzag_traversal(matrix, m, n);

    std::cout << std::endl << "Zigzag vector:" << std::endl;
    for (int val : zigzag) {
        std::cout << std::setw(8) << val;
    }
    std::cout << std::endl;

    delete[] matrix;
    return 0;
}

// Tests:
//
// test1
// input:
// m = 2, n = 3
// matrix:
// 1  2  3
// 4  5  6
// output:
// Zigzag vector: 1  2  3  6  5  4
//
// test2
// input:
// m = 3, n = 2
// matrix:
// 7  8
// 9  10
// 11 12
// output:
// Zigzag vector: 7  8  10  9  11  12
//
// test3
// input:
// m = 3, n = 3
// matrix:
// 1  2  3
// 4  5  6
// 7  8  9
// output:
// Zigzag vector: 1  2  3  6  5  4  7  8  9
//
// test4
// input:
// m = 1, n = 4
// matrix:
// 10  20  30  40
// output:
// Zigzag vector: 10  20  30  40
//
// test5
// input:
// m = 4, n = 1
// matrix:
// 1
// 2
// 3
// 4
// output:
// Zigzag vector: 1  2  3  4
//
// test6
// input:
// m = 1, n = 1
// matrix:
// 42
// output:
// Zigzag vector: 42
//
// test7
// input:
// m = 2, n = 2
// matrix:
// 5  6
// 7  8
// output:
// Zigzag vector: 5  6  8  7
//
// test8
// input:
// m = 4, n = 3
// matrix:
// 1   2   3
// 4   5   6
// 7   8   9
// 10 11  12
// output:
// Zigzag vector: 1  2  3  6  5  4  7  8  9  12 11 10
