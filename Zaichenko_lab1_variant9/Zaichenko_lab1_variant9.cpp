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
    k = 0;
    for (int i = 0; i < 16; ++i) {
		output_arr[i] = (packed & (1 << i)) != 0; // Перевірка чи i-й біт встановлений в 1
        if (output_arr[i]) {
			k++; // Лічильник кількості bits скільки поставлено в 1
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
    while (!(std::cin >> n) || n < 1 || n > 16) {
        std::cout << "Error: n must be between 1 and 16." << std::endl;
        std::cin.clear(); // Clear error flag
        std::cin.ignore(10000, '\n'); // Discard invalid input
        std::cout << "Enter number of elements (1-16): ";
    }

    bool input_arr[16] = { false };
    int bits[16] = { 0 };
    int index = 0;
    for (int i = 0; i < n; ++i) {
        std::cout << "arr[" << i << "]: ";
        int bit_position;
        if (!(std::cin >> bit_position)) {
            std::cout << "Invalid input. Please enter a number between 0 and 15: ";
            std::cin.clear(); // Очищуємо позначку помилки
            std::cin.ignore(10000, '\n'); // Прбераємо неправильний інпут
            --i;
            continue;
        }

        if (bit_position >= 0 && bit_position < 16) {
            input_arr[bit_position] = true;
            bits[index++] = bit_position;
        }
        else {
            std::cout << "Error: Bit position " << bit_position << " is out of range (0-15)." << std::endl;
            std::cout << "Please enter a valid bit position: ";
            --i;
        }
        
    }

    std::cout << "Input array: ";
    for (int i = 0; i < n; ++i) {
        if (i > 0) {
            std::cout << ",";
        }
        std::cout << bits[i];
    }
    std::cout << std::endl;

    uint16_t packed = pack(input_arr, 16);
    std::cout << "Packed number: ";
    print_binary(packed);
    std::cout << ";" << std::endl;

    bool output_arr[16] = { false };
    int k = 0;
    unpack(packed, output_arr, k);
    std::cout << "Output array: ";
    for (int i = 0; i < n; ++i) {
        if (i > 0) {
            std::cout << ",";
        }
        std::cout << bits[i];
    }
    std::cout << std::endl;

    return 0;
}



// testl
//Input array: 1, 3, 2
//Packed number : 0000 0000  0000 1110;
//Output array: 1, 3, 2
// 
// test2
//Input array: 3
//Packed number : 0000 0000  0000 1000;
//Output array: 3
// 
// test3
//Input array: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
//Packed number : 1111 1111  1111 1111;
//Output array: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
// 
// test5
//Input array: 15, 0
//Packed number : 1000 0000  0000 0001;
//Output array: 15, 0
//
// test6
//Enter number of elements(1 - 16) : 32
//Error : n must be between 1 and 16.
//Enter number of elements(1 - 16) : 1
//arr[0] :
//    1
//    Input array : 1
//    Packed number : 0000 0000  0000 0010;
//Output array: 1
// 
// test7
//Enter number of elements(1 - 16) : df
//Error : n must be between 1 and 16.
//Enter number of elements(1 - 16) : 1
//arr[0] : 1
//Input array : 1
//Packed number : 0000 0000  0000 0010;
//Output array: 1
//
// test8
//Enter number of elements(1 - 16) : 1
//arr[0] : sd
//Invalid input.Please enter a number between 0 and 15 : arr[0] : 1
//Input array : 1
//Packed number : 0000 0000  0000 0010;
//Output array: 1