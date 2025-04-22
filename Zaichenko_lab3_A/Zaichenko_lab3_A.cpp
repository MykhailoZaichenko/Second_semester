#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;


// Функція для знаходження характеристики рядка (мінімум за абсолютним значенням)
double row_characteristic(double* row, int n) {
    double min_val = fabs(row[0]);
    for (int i = 1; i < n; ++i) {
        if (fabs(row[i]) < min_val) {
            min_val = fabs(row[i]);
        }
    }
    return min_val;
}

// Вставкове сортування рядків матриці за спаданням характеристик
void insertion_sort(double** matrix, int m, int n) {
    for (int i = 1; i < m; ++i) {
        double* current_row = matrix[i];
        double current_char = row_characteristic(current_row, n);
        int j = i - 1;
        while (j >= 0 && row_characteristic(matrix[j], n) < current_char) {
            matrix[j + 1] = matrix[j]; // Зсуваємо рядки вниз
            j--;
        }
        matrix[j + 1] = current_row;
    }
}

// Вивід матриці
void print_matrix(double** matrix, int m, int n) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << setw(8) << matrix[i][j];
        }
        cout << '\n';
    }
}

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


    // Динамічно виділяємо пам’ять
    double** matrix = new double* [m];
    for (int i = 0; i < m; ++i) {
        matrix[i] = new double[n];
    }

    cout << "Enter matrix elements:" << endl;
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            cout << "matrix[" << i << "][" << j << "] ";
			while (!(cin >> matrix[i][j])) {
				cout << "Invalid input. Please enter a number: ";
				cin.clear();
				cin.ignore(10000, '\n');
			}
        }
    }
    cout << "\nInitial matrix:" << endl;
    print_matrix(matrix, m, n);

    insertion_sort(matrix, m, n);

    cout << "\nSorted matrix by row characteristics (descending):" << endl;
    print_matrix(matrix, m, n);

    // Звільнення пам’яті
    for (int i = 0; i < m; ++i)
        delete[] matrix[i];
    delete[] matrix;

    return 0;
}

// Tests:
//
// test1
// input:
// m = 3, n = 3
// matrix:
// 1  2   3
// -1 2  -3
// -2 3   4
// output after sorting:
// -2  3   4       // characteristic = 2
// 1   2   3       // characteristic = 1
// -1  2  -3       // characteristic = 1
//
// test2
// input:
// m = 2, n = 4
// matrix:
// 5  -6  0  1
// 2   0  3  4
// output after sorting:
// 2   0   3   4    // characteristic = 0
// 5  -6   0   1    // characteristic = 0
//
// test3
// input:
// m = 4, n = 2
// matrix:
// -3  4
// -1 -1
//  0  5
// -2  1
// output after sorting:
// -1 -1        // characteristic = 1
// -2  1        // characteristic = 1
// -3  4        // characteristic = 3
// 0   5        // characteristic = 0
//
// test4
// input:
// m = 1, n = 3
// matrix:
// -7  2  0
// output after sorting:
// -7  2  0        // no change since only one row
//
// test5
// input:
// m = 3, n = 1
// matrix:
// -5
// -1
//  0
// output after sorting:
// -1       // characteristic = 1
//  0       // characteristic = 0
// -5       // characteristic = 5
//
// test6
// input:
// m = 2, n = 2
// matrix:
// -2 2
// -2 2
// output after sorting:
// -2 2
// -2 2        // same characteristics, original order preserved
