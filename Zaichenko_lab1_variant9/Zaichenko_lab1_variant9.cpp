//Варіант 9
//Дано масив n(1 <= n <= 16) значень логічного типу(true / fatse).
//1.	Реалізувати функцію для ïx упакування в беззнакове(unsigned) 16 - bit число, в якому k - и біт приймас значения 1, тоді i тільки тоді, коли k - й елемент масиву містить значення true.Функція повинна мати наступну сигнатуру :
//uint16_t pack(bool* input_arr, int n)
//2.	Реалізувати  функцію  для  розпакування  чисел  із  ïx  упакованого
//беззнакового(unsigned) 16 - bit представлення в масив цілих чисел.Після розпакування початковий порядок чисел повинен відновитись.Функціs повинна мати наступну сигнатуру :
//void unpack(uintl6_t packed, bool* output_arr, int& k}
//3.	Реаліsувати функцііо для виведення в консоль бітового представлення упакованого числа.ІІід час виведення кожні 4 біти числа повинні бути розділені прогалиноіо(space), а кожні 8 бітів двома прогалинами.Функія повинна мати наступну сигнатуру :
//void print_binary(uint16_t nuшber)

#include <iostream>
#include <cstdint>
#include <sstream>

// Функція для упаковки логічного масиву в 16-бітне число
uint16_t pack(bool* input_arr, int n) {
    uint16_t packed = 0;
    for (int i = 0; i < n; ++i) {
        if (input_arr[i]) {
            packed |= (1 << i);
        }
    }
    return packed;
}

// Функція для розпаковки 16-бітного числа в масив логічних значень
void unpack(uint16_t packed, bool* output_arr, int& k, int* indices) {
    k = 0;
    for (int i = 0; i < 16; ++i) {
        output_arr[i] = (packed & (1 << i)) != 0;
        if (output_arr[i]) {
            indices[k++] = i;
        }
    }
}


// Функція для виведення бітового представлення числа
void print_binary(uint16_t number) {
    for (int i = 15; i >= 0; --i) {
        std::cout << ((number >> i) & 1);
        if (i % 4 == 0) std::cout << ' ';
        if (i % 8 == 0) std::cout << ' ';
    }
    std::cout << std::endl;
}

int main() {
    const int ARRAY_SIZE = 16;

    bool input_arr[ARRAY_SIZE] = { false };
    std::string input;
    std::cout << "Enter numbers (bit positions 0-15), separated by commas: ";
    std::cin >> std::ws; // Очистка зайвих пробілів
    std::getline(std::cin, input);

    std::stringstream ss(input);
    std::string value;
    int indices[ARRAY_SIZE] = { 0 };
    int index = 0;
    while (std::getline(ss, value, ',')) {
        int bit_position = std::stoi(value);
        if (bit_position >= 0 && bit_position < ARRAY_SIZE) {
            input_arr[bit_position] = true;
            indices[index++] = bit_position;
        }
    }

    uint16_t packed = pack(input_arr, ARRAY_SIZE);
    std::cout << "Packed value: ";
    print_binary(packed);

    bool output_arr[ARRAY_SIZE] = { false };
    int k = 0;
    int unpacked_indices[ARRAY_SIZE] = { 0 };
    unpack(packed, output_arr, k, unpacked_indices);

    std::cout << "Unpacked values: ";
    for (int i = 0; i < index; ++i) {
        if (i > 0) {
            std::cout << ",";
        }
        std::cout << indices[i];
    }
    std::cout << std::endl;

    return 0;
}







// testl
// input: 1,3,2
// output: 00011110; 1,3,2
// test2
// input: 3
// output: 00000011; 3 
// test3
// input: 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
// output: 11111111 11111111; 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
// test4
// input: -
// output: 00000000 00000000; -
// test5
// input: 0, 15
// output: 10000000 00000001; 0,15