//Варіант 9
//Дано масив n(1 <= n <= 16) значень логічного типу(true / fatse).


#include <iostream>
#include <cstdint>

//1.	Реалізувати функцію для ïx упакування в беззнакове(unsigned) 16 - bit число, в якому k - и біт приймас значения 1, тоді i тільки тоді, коли k - й елемент масиву містить значення true.Функція повинна мати наступну сигнатуру :
//uint16_t pack(bool* input_arr, int n)
uint16_t pack(bool* input_arr, int n) {
    uint16_t packed = 0;
    for (int i = 0; i < n; ++i) {
        if (input_arr[i]) {
			packed |= (1 << i); // Bitwise OR Встановлює i-й біт у 1
        }
    }
    return packed;
}

//2.	Реалізувати  функцію  для  розпакування  чисел  із  ïx  упакованого
//беззнакового(unsigned) 16 - bit представлення в масив цілих чисел.Після розпакування початковий порядок чисел повинен відновитись.Функціs повинна мати наступну сигнатуру :
//void unpack(uintl6_t packed, bool* output_arr, int& k}
void unpack(uint16_t packed, bool* output_arr, int& k) {

    for (int i = 0; i < 16; ++i) {
		output_arr[i] = (packed & (1 << i)) != 0; // Перевірка чи i-й біт встановлений в 1
        if (output_arr[i]) {

        }
    }
}

//3.	Реаліsувати функцііо для виведення в консоль бітового представлення упакованого числа.ІІід час виведення кожні 4 біти числа повинні бути розділені прогалиноіо(space), а кожні 8 бітів двома прогалинами.Функія повинна мати наступну сигнатуру :
//void print_binary(uint16_t nuшber)
void print_binary(uint16_t number) {
    for (int i = 15; i >= 0; --i) {
        std::cout << ((number >> i) & 1);
        if (i % 4 == 0 && i != 0) std::cout << ' ';
        if (i % 8 == 0 && i != 0) std::cout << ' ';
    }
}

int main() {
    const int ARRAY_SIZE = 16;

    int n;
    std::cout << "Enter number of elements (1-16): ";
    while (!(std::cin >> n) || n < 1 || n > 16 || std::cin.peek() != '\n') {
        std::cout << "Error: n must be a valid integer between 1 and 16." << std::endl;
        std::cin.clear(); // Clear error flag
        std::cin.ignore(10000, '\n');
        std::cout << "Enter number of elements (1-16): ";
    }

    bool input_arr[ARRAY_SIZE];
    std::cout << std::boolalpha;
    for (int i = 0; i < n; ++i) {
        std::cout << "arr[" << i << "] (true/false): ";
        bool value;
        while (!(std::cin >> std::boolalpha >> value)) {
            std::cout << "Invalid input. Please enter 'true' or 'false': ";
            std::cin.clear(); // Clear error flag
            std::cin.ignore(10000, '\n'); // Discard invalid input
        }
        input_arr[i] = value;
    }

    // Display input values
    std::cout << "Input array: ";
    for (int i = 0; i < n; ++i) {
        if (i > 0) {
            std::cout << ", ";
        }
        std::cout << input_arr[i];
    }
    std::cout << std::endl;

    uint16_t packed = pack(input_arr, n);
    std::cout << "Packed number: ";
    print_binary(packed);
    std::cout << ";" << std::endl;

    bool output_arr[ARRAY_SIZE];
    int k = 0;
    unpack(packed, output_arr, k);
    std::cout << "Output array: ";
    for (int i = 0; i < n; ++i) {
        if (i > 0) {
            std::cout << ", ";
        }
        std::cout << input_arr[i];
    }
    std::cout << std::endl;

    return 0;
}




// testl
//Enter number of elements(1 - 16) : 3
//arr[0](true / false) : true
//arr[1](true / false) : false
//arr[2](true / false) : false
//Input array : true, false, false
//Packed number : 0000 0000  0000 0001;
//Output array: true, false, false
// 
// test2
//Enter number of elements(1 - 16) : 1
//arr[0](true / false) : true
//Input array : true
//Packed number : 0000 0000  0000 0001;
//Output array: true
// 
// test3
//Input array: true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true
//Packed number : 1111 1111  1111 1111;
//Output array: true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true
//
// test6
//Enter number of elements(1 - 16) : 43
//Error : n must be a valid integer between 1 and 16.
//Enter number of elements(1 - 16) : 1
//arr[0](true / false) : true
//Input array : true
//Packed number : 0000 0000  0000 0001;
//Output array: true
// 
// test7
//Enter number of elements(1 - 16) : df
//Error : n must be a valid integer between 1 and 16.
//Enter number of elements(1 - 16) : 1
//arr[0](true / false) : true
//Input array : true
//Packed number : 0000 0000  0000 0001;
//Output array: true
//
// test8
//Enter number of elements(1 - 16) : 1
//arr[0](true / false) : df
//Invalid input.Please enter 'true' or 'false' : true
//Input array : true
//Packed number : 0000 0000  0000 0001;
//Output array: true