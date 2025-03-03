//Варіант 9
//Дано масив n(1 <= n <= 16) значень логічного типу(true / fatse).


#include <iostream>
#include <cstdint>

//1.	Реалізувати функцію для ïx упакування в беззнакове(unsigned) 16 - bit число, в якому k - и біт приймас значения 1, тоді i тільки тоді, коли k - й елемент масиву містить значення true.Функція повинна мати наступну сигнатуру :
//uint16_t pack(bool* input_arr, int n)
uint16_t pack(bool* input_arr, int n) {
    uint16_t packed = 0;
    for (int i = 0; i < n; i++) {
        if (input_arr[i]) {
            packed |= (1 << i);
        }
    }
    return packed;
}

//2.	Реалізувати  функцію  для  розпакування  чисел  із  ïx  упакованого
//беззнакового(unsigned) 16 - bit представлення в масив цілих чисел.Після розпакування початковий порядок чисел повинен відновитись.Функціs повинна мати наступну сигнатуру :
//void unpack(uintl6_t packed, bool* output_arr, int& k}
void unpack(uint16_t packed, bool* output_arr, int& k) {
    for (int i = 0; i < 16; i++) {
        output_arr[i] = (packed & (1 << i));
        if (output_arr[i]) {
            k++;
        }
    }
}

//3.	Реаліsувати функцііо для виведення в консоль бітового представлення упакованого числа.ІІід час виведення кожні 4 біти числа повинні бути розділені прогалиноіо(space), а кожні 8 бітів двома прогалинами.Функія повинна мати наступну сигнатуру :
//void print_binary(uint16_t nuшber)
void print_binary(uint16_t number) {
    for (int i = 15; i >= 0; i--) {
        std::cout << ((number >> i) & 1);
        if (i % 4 == 0 && i != 0) std::cout << ' ';
        if (i % 8 == 0 && i != 0) std::cout << ' ';
    }
}

int main() {
    const int ARRAY_SIZE = 16;

    int n;
    std::cout << "Enter number of elements (1-16): ";
    std::cin >> n;

    if (n < 1 || n > 16) {
        std::cout << "Error: n must be between 1 and 16." << std::endl;
        return 1;
    }

    bool input_arr[16] = { false };
    std::cout << "Enter " << n << " bit positions (0-15), separated by spaces: ";

    int bits[16] = { 0 };
    int index = 0;
    for (int i = 0; i < n; ++i) {
        int bit_position;
        std::cin >> bit_position;
        if (bit_position >= 0 && bit_position < 16) {
            input_arr[bit_position] = true;
            bits[index++] = bit_position;
        }
        else {
            std::cerr << "Error: Bit position " << bit_position << " is out of range (0-15)." << std::endl;
            return 1;
        }
    }

    //std::cout << "Initial array: ";
    //for (int i = 0; i < 16; ++i) {
    //    std::cout << input_arr[i] << " ";
    //}
    //std::cout << std::endl;

    uint16_t packed = pack(input_arr, 16); // Fix: Use full array size for packing
    std::cout << "Output: ";
    print_binary(packed);
    std::cout << "; ";

    bool output_arr[16] = { false };
    int k = 0;
    unpack(packed, output_arr, k);

    for (int i = 0; i < index; ++i) {
        if (i > 0) {
            std::cout << ",";
        }
        std::cout << bits[i];
    }
    std::cout << std::endl;

    return 0;
}



// testl
// Input bit position, separated by commas : 1, 3, 2
// Output : 0000 0000  0000 1110; 1, 3, 2
// 
// test2
// Input bit position, separated by commas : 3
// Output : 0000 0000  0000 1000; 3
// 
// test3
// Input bit position, separated by commas : 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
// Output : 1111 1111  1111 1111; 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
// test4
// Input bit position, separated by commas :
// Output: 0000 0000  0000 0000;
// 
// test5
// Input bit position, separated by commas : 0, 15
// Output : 1000 0000  0000 0001; 0, 15
//
// test6
// Input bit position, separated by commas : 234
// Error: Bit position is out of range (0-15).
//
// test7
// Input bit position, separated by commas : sdf
// Invalid input: non-numeric character detected.

//test8
// Input bit position, separated by commas : 4f44
// Invalid input : non - numeric character detected.