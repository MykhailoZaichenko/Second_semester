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
        double current_charac = row_characteristic(current_row, n);
        int prev_row = i - 1;
        while (prev_row >= 0 && row_characteristic(matrix[prev_row], n) < current_charac) {
            matrix[prev_row + 1] = matrix[prev_row]; // Зсуваємо рядки вниз
            prev_row--;
        }
        matrix[prev_row + 1] = current_row;
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

// Tests
// 
// test1
// m = 3, n = 3
// Initial matrix:
// 1       2       3
// -1      2      -3
// -2      3       4
//
// Sorted matrix by row characteristics (descending):
// -2      3       4
// 1       2       3
// -1      2      -3

// test2
// m = 2, n = 4
// Initial matrix:
// -5     -6     -7     -8
// 1       0       2       3
//
// Sorted matrix by row characteristics (descending):
// 1       0       2       3
// -5     -6     -7     -8

// test3
// m = 4, n = 2
// Initial matrix:
// 7      -7
// 5       5
// -1     -2
// 0       0
//
// Sorted matrix by row characteristics (descending):
// 7      -7
// 5       5
// -1     -2
// 0       0

// test4
// m = 3, n = 3
// Initial matrix:
// -2     -3     -4
// -1     -1     -1
//  2      3      4
//
// Sorted matrix by row characteristics (descending):
// -2     -3     -4
//  2      3      4
// -1     -1     -1

// test5
// m = 2, n = 2
// Initial matrix:
// -1      1
// -0.5   0.5
//
// Sorted matrix by row characteristics (descending):
// -1      1
// -0.5   0.5

// test6
// m = 1, n = 5
// Initial matrix:
// -10     -20     -30     -40     -50
//
// Sorted matrix by row characteristics (descending):
// -10     -20     -30     -40     -50

// test7
// m = 1, n = 1
//Initial matrix :
//    0
//
//Sorted matrix by row characteristics(descending) :
//    0