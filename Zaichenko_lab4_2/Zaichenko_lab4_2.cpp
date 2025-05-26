#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>
#include <map>
#include <vector>
#include <iomanip>

struct Studio {
    std::string studio_name;
    std::string website;
    std::string country;
};

struct Movie {
    std::string title;
    int release_year;
    int runtime_minutes;
    std::string synopsis;
    std::string rating;
    Studio studio_info;
};

struct Node {
    Movie data;
    Node* next;

    Node(const Movie& movie) : data(movie), next(nullptr) {}
};

// Структура для зберігання результату розбору рядка на токени
struct TokenResult {
    std::string token;          // Отриманий токен
    size_t next_pos;           // Позиція початку наступного токену
};

// Функція для отримання наступного токену з рядка
TokenResult get_next_token(const std::string& str, size_t start_pos, char delim) {
    // Перевірка чи не вийшли за межі рядка
    if (start_pos >= str.length()) {
        return { "", str.length() };
    }

    // Пошук наступного роздільника
    size_t end_pos = str.find(delim, start_pos);
    if (end_pos == std::string::npos) {
        return { str.substr(start_pos), str.length() };
    }

    return { str.substr(start_pos, end_pos - start_pos), end_pos + 1 };
}

// Вузол однозв'язного списку для зберігання студій
struct StudioNode {
    int id;                     // Ідентифікатор студії
    Studio data;                // Дані про студію
    StudioNode* next;           // Вказівник на наступний елемент списку

    // Конструктор вузла
    StudioNode(int studio_id, const Studio& studio) : id(studio_id), data(studio), next(nullptr) {}
};

StudioNode* read_studios(std::ifstream& studios_file) {
    StudioNode* head = nullptr;
    std::string line;

    // Пропускаємо заголовок файлу
    std::getline(studios_file, line);

    while (std::getline(studios_file, line)) {
        // Розбираємо рядок на токени
        size_t pos = 0;
        TokenResult id_token = get_next_token(line, pos, '\t');
        TokenResult name_token = get_next_token(line, id_token.next_pos, '\t');
        TokenResult website_token = get_next_token(line, name_token.next_pos, '\t');
        TokenResult country_token = get_next_token(line, website_token.next_pos, '\t');

        // Створюємо новий вузол та додаємо його до списку
        int id = std::stoi(id_token.token);
        Studio studio = { name_token.token, website_token.token, country_token.token };

        StudioNode* new_node = new StudioNode(id, studio);
        new_node->next = head;
        head = new_node;
    }
    return head;
}

void delete_studios(StudioNode* head) {
    while (head != nullptr) {
        StudioNode* temp = head;
        head = head->next;
        delete temp;
    }
}

Studio* find_studio(StudioNode* studios, int id) {
    while (studios != nullptr) {
        if (studios->id == id) {
            return &studios->data;
        }
        studios = studios->next;
    }
    return nullptr;
}

Node* read_from_tsv(std::ifstream& movies_file, std::ifstream& studios_file) {
    StudioNode* studios = read_studios(studios_file);

    std::string line;
    Node* head = nullptr;
    Node* tail = nullptr;

    // Пропускаємо заголовок файлу фільмів
    std::getline(movies_file, line);

    // Читаємо дані про фільми
    while (std::getline(movies_file, line)) {
        // Розбираємо рядок на токени
        size_t pos = 0;
        TokenResult id_token = get_next_token(line, pos, '\t');
        TokenResult title_token = get_next_token(line, id_token.next_pos, '\t');
        TokenResult year_token = get_next_token(line, title_token.next_pos, '\t');
        TokenResult runtime_token = get_next_token(line, year_token.next_pos, '\t');
        TokenResult synopsis_token = get_next_token(line, runtime_token.next_pos, '\t');
        TokenResult rating_token = get_next_token(line, synopsis_token.next_pos, '\t');
        TokenResult studio_id_token = get_next_token(line, rating_token.next_pos, '\t');

        // Створюємо об'єкт фільму
        Movie movie;
        movie.title = title_token.token;
        movie.release_year = std::stoi(year_token.token);
        movie.runtime_minutes = std::stoi(runtime_token.token);
        movie.synopsis = synopsis_token.token;
        movie.rating = rating_token.token;

        // Знаходимо та додаємо інформацію про студію
        int studio_id = std::stoi(studio_id_token.token);
        Studio* studio = find_studio(studios, studio_id);
        if (studio != nullptr) {
            movie.studio_info = *studio;
        }

        // Створюємо новий вузол та додаємо його до списку
        Node* new_node = new Node(movie);

        if (head == nullptr) {
            head = new_node;
            tail = new_node;
        }
        else {
            tail->next = new_node;
            tail = new_node;
        }
    }

    // Звільняємо пам'ять списку студій
    delete_studios(studios);
    return head;
}

// Предикат для фільтрації індійських фільмів після 2000 року тривалістю більше 60 хвилин
bool is_indian_long_movie_after_2000(const Movie& movie) {
    return movie.studio_info.country == "India" &&
        movie.runtime_minutes > 60 &&
        movie.release_year > 2000;
}

// Функція для видалення списку фільмів та звільнення пам'яті
void delete_list(Node* head) {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

// Функція для вилучення елементів, що задовольняють предикату
Node* extract_all(Node*& head, bool(*pred)(const Movie&)) {
    Node* extracted_head = nullptr;
    Node* extracted_tail = nullptr;
    Node* current = head;
    Node* prev = nullptr;

    while (current != nullptr) {
        if (pred(current->data)) {
            // Видаляємо з початкового списку
            if (prev == nullptr) {
                head = current->next;
            }
            else {
                prev->next = current->next;
            }

            // Додаємо до списку вилучених елементів
            Node* next = current->next;
            current->next = nullptr;

            if (extracted_head == nullptr) {
                extracted_head = current;
                extracted_tail = current;
            }
            else {
                extracted_tail->next = current;
                extracted_tail = current;
            }

            current = next;
        }
        else {
            prev = current;
            current = current->next;
        }
    }

    return extracted_head;
}

void print_list(std::ostream& out_stream, const Node* head) {
    if (head == nullptr) {
        out_stream << u8"Список порожній\n";
        return;
    }

    while (head != nullptr) {
        const Movie& movie = head->data;
        out_stream << u8"Назва: " << movie.title << "\n"
            << u8"Рік: " << movie.release_year << "\n"
            << u8"Тривалість: " << movie.runtime_minutes << u8" хв.\n"
            << u8"Рейтинг: " << movie.rating << "\n"
            << u8"Студія: " << movie.studio_info.studio_name << "\n"
            << u8"Країна: " << movie.studio_info.country << "\n"
            << u8"Веб-сайт: " << movie.studio_info.website << "\n"
            << u8"Опис: " << movie.synopsis << "\n"
            << "----------------------------------------\n";
        head = head->next;
    }
}

void write_to_tsv(std::ofstream& out_file, const Node* head) {
    // Записуємо заголовок файлу
    out_file << "Title\tYear\tRuntime\tSynopsis\tRating\tStudio\tCountry\tWebsite\n";

    // Записуємо дані про кожен фільм
    while (head != nullptr) {
        const Movie& movie = head->data;
        out_file << movie.title << "\t"
            << movie.release_year << "\t"
            << movie.runtime_minutes << "\t"
            << movie.synopsis << "\t"
            << movie.rating << "\t"
            << movie.studio_info.studio_name << "\t"
            << movie.studio_info.country << "\t"
            << movie.studio_info.website << "\n";
        head = head->next;
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    std::ifstream movies_file("movies.tsv");
    std::ifstream studios_file("studios.tsv");

    if (!movies_file.is_open() || !studios_file.is_open()) {
        std::cout << u8"Помилка: Неможливо відкрити вхідні файли\n";
        return 1;
    }

    Node* head = read_from_tsv(movies_file, studios_file);

    std::cout << u8"\nПочатковий список фільмів:\n";
    print_list(std::cout, head);

    Node* extracted = extract_all(head, is_indian_long_movie_after_2000);

    std::cout << u8"\nСписок після вилучення:\n";
    print_list(std::cout, head);

    std::cout << u8"\nВилучені фільми:\n";
    print_list(std::cout, extracted);

    std::ofstream out_file("extracted_movies.tsv");
    if (out_file.is_open()) {
        write_to_tsv(out_file, extracted);
    }

    delete_list(head);
    delete_list(extracted);

    movies_file.close();
    studios_file.close();
    out_file.close();

    return 0;
}
/*
* Тестові сценарії для програми обробки фільмів
* ============================================
*
* Тест 1: Порожні файли
* Вхідні дані:
* - movies.tsv: тільки заголовок
* - studios.tsv: тільки заголовок
* Очікуваний результат:
* - Порожній початковий список
* - Порожній список вилучених елементів
* - Порожній вихідний файл
*
* Тест 2: Список без індійських фільмів
* Вхідні дані:
* - movies.tsv: 5 фільмів (всі не індійські)
* - studios.tsv: студії з різних країн
* Очікуваний результат:
* - Початковий список: 5 фільмів
* - Список вилучених елементів: порожній
* - Вихідний список: 5 фільмів (без змін)
*
* Тест 3: Список тільки з індійськими фільмами після 2000 року
* Вхідні дані:
* - movies.tsv: 3 індійські фільми (всі після 2000, довше 60 хв)
* - studios.tsv: включає індійські студії
* Очікуваний результат:
* - Початковий список: 3 фільми
* - Всі фільми вилучені
* - Вихідний список: порожній
*
* Тест 4: Змішаний список (поточний тест)
* Вхідні дані:
* - movies.tsv: 10 фільмів (4 індійські після 2000 року, довше 60 хв)
* - studios.tsv: 8 студій з різних країн
* Очікуваний результат:
* - Початковий список: 10 фільмів
* - Вилучено: 4 індійські фільми
* - Залишилось: 6 фільмів
*
* Тест 5: Граничні випадки
* - Фільм рівно 60 хвилин (не має бути вилучений)
* - Фільм 2000 року (не має бути вилучений)
* - Індійський фільм до 2000 року
* - Індійський фільм менше 60 хвилин
* Очікуваний результат: жоден з фільмів не має бути вилучений
*
*/