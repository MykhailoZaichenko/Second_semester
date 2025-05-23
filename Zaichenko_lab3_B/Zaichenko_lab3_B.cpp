#include <iostream>
#include <iomanip>
using namespace std;

// Функція для формування зигзагового масиву (без vector)
int* zigzag_sorting(int* matrix, int m, int n, int& size) {
    int* zigzag = new int[m * n];  // Виділяємо максимальну можливу кількість елементів
    size = 0;

    for (int diag_shift = n - 1; diag_shift >= -(m - 1); --diag_shift) {
        if ((n - 1 - diag_shift) % 2 == 0) {
            // Парна діагональ — знизу вгору
            for (int i = m - 1; i >= 0; --i) {
                int j = diag_shift + i;
                if (j >= 0 && j < n) {
                    zigzag[size++] = matrix[i * n + j];
                }
            }
        }
        else {
            // Непарна діагональ — згори вниз
            for (int i = 0; i < m; ++i) {
                int j = diag_shift + i;
                if (j >= 0 && j < n) {
                    zigzag[size++] = matrix[i * n + j];
                }
            }
        }
    }

    return zigzag;
}

// Функція для виводу матриці
void print_matrix(int* matrix, int m, int n) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << setw(8) << matrix[i * n + j];
        }
        cout << endl;
    }
}

// Функція для виводу зигзагового вектора
int main() {
    int m, n;
    cout << "Input rows (m > 0): ";
    while (!(cin >> m) || m <= 0) {
        cout << "Error: m must be a positive integer." << endl;
        cout << "Input rows (m > 0): ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    cout << "Input cols (n > 0): ";
    while (!(cin >> n) || n <= 0) {
        cout << "Error: n must be a positive integer." << endl;
        cout << "Input columns (n > 0): ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    int* matrix = new int[m * n]; // Неперервна пам’ять (row-major)

    cout << "Enter matrix elements:" << endl;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << "matrix[" << i << "][" << j << "] ";
            cin >> matrix[i * n + j];
        }
    }

    cout << "\nInitial matrix:" << endl;
    print_matrix(matrix, m, n);

    int size = 0;
    int* zigzag = zigzag_sorting(matrix, m, n, size);

    cout << "\nZigzag vector:" << endl;
    for (int i = 0; i < size; ++i) {
        cout << setw(8) << zigzag[i];
    }
    cout << endl;

    delete[] matrix;
    delete[] zigzag;
    return 0;
}

// Tests:
//
// test1
// m = 2, n = 2
// Initial matrix:
// 1       2
// 3       4
//
// Zigzag vector:
// 2       1       4       3

// test2
// m = 3, n = 3
// Initial matrix:
// 1       2       3
// 4       5       6
// 7       8       9
//
// Zigzag vector:
// 3       2       6       9       5       1       4       8       7

// test3
// m = 4, n = 4
// Initial matrix:
// 10     11      12      13
// 14     15      16      17
// 18     19      20      21
// 22     23      24      25
//
// Zigzag vector:
// 13      12      17      21      16      11      10      15      20      25      24      19      14      18      23      22

// test4
// m = 1, n = 5
// Initial matrix:
// 1       2       3       4       5
//
// Zigzag vector:
// 5       4       3       2       1

// test5
// m = 5, n = 1
// Initial matrix:
// 1
// 2
// 3
// 4
// 5
//
// Zigzag vector:
// 1       2       3       4       5

// test6
// m = 3, n = 4
// Initial matrix:
// 1       2       3       4
// 5       6       7       8
// 9      10      11      12
//
// Zigzag vector:
// 4       3       8       12      7       2       1       6       11      10      5       9

// test7
// m = 4, n = 3
// Initial matrix:
// 1       2       3
// 4       5       6
// 7       8       9
// 10     11      12
//
// Zigzag vector:
// 3       2       6       9       5       1       4       8       12      11      7       10

// test8
// m = 1, n = 1
//Initial matrix :
// 0
//
//Zigzag vector :
// 0