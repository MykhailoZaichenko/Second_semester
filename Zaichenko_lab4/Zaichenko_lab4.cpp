#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <windows.h> // для SetConsoleOutputCP

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
};

/**
 * @brief Зчитує дані про фільми та студії з файлів і формує однозв'язний список.
 */
Node* read_from_tsv(std::ifstream& movies_file, std::ifstream& studios_file) {
    std::string line;
    Node* head = nullptr;
    Node* tail = nullptr;

    std::getline(studios_file, line); // заголовок studios
    std::getline(movies_file, line);  // заголовок movies

    // Читання studios у масив
    const int MAX_STUDIOS = 1000;
    Studio studios[MAX_STUDIOS];
    int studio_count = 0;
    while (std::getline(studios_file, line) && studio_count < MAX_STUDIOS) {
        std::stringstream ss(line);
        std::getline(ss, studios[studio_count].studio_name, '\t');
        std::getline(ss, studios[studio_count].website, '\t');
        std::getline(ss, studios[studio_count].country);
        ++studio_count;
    }

    // Читання movies у список
    while (std::getline(movies_file, line)) {
        std::stringstream ss(line);
        Movie m;
        std::string year, runtime, studio_id_str;

        std::getline(ss, m.title, '\t');
        std::getline(ss, year, '\t');
        std::getline(ss, runtime, '\t');
        std::getline(ss, m.synopsis, '\t');
        std::getline(ss, m.rating, '\t');
        std::getline(ss, studio_id_str);

        m.release_year = std::stoi(year);
        m.runtime_minutes = std::stoi(runtime);
        int studio_id = std::stoi(studio_id_str);
        if (studio_id > 0 && studio_id <= studio_count) {
            m.studio_info = studios[studio_id - 1];
        }

        Node* new_node = new Node{ m, nullptr };
        if (!head) {
            head = tail = new_node;
        }
        else {
            tail->next = new_node;
            tail = new_node;
        }
    }

    return head;
}

/**
 * @brief Умова для вилучення фільмів (18+, з Німеччини, без синопсису)
 */
bool german_adult_without_synopsis(const Movie& m) {
    return m.rating == "18+" && m.studio_info.country == "Germany" && m.synopsis.empty();
}

/**
 * @brief Вилучає вузли, що відповідають предикату, в новий список.
 */
Node* extract_all(Node*& head, bool (*pred)(const Movie&)) {
    Node* extracted_head = nullptr;
    Node* extracted_tail = nullptr;
    Node* current = head;
    Node* prev = nullptr;

    while (current) {
        if (pred(current->data)) {
            Node* to_remove = current;
            current = current->next;

            if (prev) prev->next = current;
            else head = current;

            to_remove->next = nullptr;
            if (!extracted_head) {
                extracted_head = extracted_tail = to_remove;
            }
            else {
                extracted_tail->next = to_remove;
                extracted_tail = to_remove;
            }
        }
        else {
            prev = current;
            current = current->next;
        }
    }

    return extracted_head;
}

/**
 * @brief Виводить список у потік.
 */
void print_list(std::ostream& os, const Node* head) {
    while (head) {
        os << std::left << std::setw(20) << head->data.title
            << std::setw(8) << head->data.release_year
            << std::setw(6) << head->data.runtime_minutes
            << std::setw(25) << (head->data.synopsis.empty() ? "(none)" : head->data.synopsis)
            << std::setw(6) << head->data.rating
            << std::setw(15) << head->data.studio_info.studio_name
            << std::setw(20) << head->data.studio_info.country << '\n';
        head = head->next;
    }
}

/**
 * @brief Записує список у TSV-файл.
 */
void write_to_tsv(std::ofstream& out, const Node* head) {
    out << "title\trelease_year\truntime_minutes\tsynopsis\trating\tstudio_name\twebsite\tcountry\n";
    while (head) {
        out << head->data.title << '\t'
            << head->data.release_year << '\t'
            << head->data.runtime_minutes << '\t'
            << head->data.synopsis << '\t'
            << head->data.rating << '\t'
            << head->data.studio_info.studio_name << '\t'
            << head->data.studio_info.website << '\t'
            << head->data.studio_info.country << '\n';
        head = head->next;
    }
}

/**
 * @brief Звільняє пам’ять списку.
 */
void free_list(Node* head) {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8); // Встановити кодування виводу UTF-8

    std::ifstream movies_file("movies.tsv");
    std::ifstream studios_file("studios.tsv");

    if (!movies_file.is_open() || !studios_file.is_open()) {
        std::cerr << "Error opening input files." << std::endl;
        return 1;
    }

    Node* movies = read_from_tsv(movies_file, studios_file);
    movies_file.close();
    studios_file.close();

    std::cout << "Original list:\n";
    print_list(std::cout, movies);

    Node* removed = extract_all(movies, german_adult_without_synopsis);

    std::cout << "\nExtracted list:\n";
    print_list(std::cout, removed);

    std::cout << "\nRemaining list:\n";
    print_list(std::cout, movies);

    std::ofstream out_file("extracted_movies.tsv");
    if (!out_file.is_open()) {
        std::cerr << "Failed to create output file.\n";
        return 2;
    }
    write_to_tsv(out_file, removed);
    out_file.close();

    free_list(movies);
    free_list(removed);
    return 0;
}
