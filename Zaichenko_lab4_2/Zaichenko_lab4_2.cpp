#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <Windows.h> // Для SetConsoleOutputCP

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

Node* read_from_tsv(std::ifstream& movies_file, std::ifstream& studios_file) {
    std::string line;
    Node* head = nullptr;
    Node* tail = nullptr;

    std::getline(studios_file, line); // заголовок studios
    std::getline(movies_file, line);  // заголовок movies

    Studio studios[1000];
    int studio_count = 0;
    while (std::getline(studios_file, line)) {
        std::stringstream ss(line);
        std::getline(ss, studios[studio_count].studio_name, '\t');
        std::getline(ss, studios[studio_count].website, '\t');
        std::getline(ss, studios[studio_count].country);
        ++studio_count;
    }

    while (std::getline(movies_file, line)) {
        std::stringstream ss(line);
        Movie m;
        std::string year_str, runtime_str, studio_id_str;

        std::getline(ss, m.title, '\t');
        std::getline(ss, year_str, '\t');
        std::getline(ss, runtime_str, '\t');
        std::getline(ss, m.synopsis, '\t');
        std::getline(ss, m.rating, '\t');
        std::getline(ss, studio_id_str);

        try {
        m.release_year = std::stoi(year_str);
        m.runtime_minutes = std::stoi(runtime_str);
        int studio_id = std::stoi(studio_id_str);
        
        if (studio_id > 0 && studio_id <= studio_count) {
			m.studio_info = studios[studio_id - 1];
		}
		else {
			std::cerr << "Помилка: Неправильний ID студії у рядку: " << line << std::endl;
			continue; // пропустити цей рядок
		}
		}
		catch (const std::invalid_argument&) {
			std::cerr << "Помилка: Неправильний формат даних у рядку: " << line << std::endl;
			continue; // пропустити цей рядок
		}
		catch (const std::out_of_range&) {
			std::cerr << "Помилка: Значення виходить за межі допустимого діапазону у рядку: " << line << std::endl;
			continue; // пропустити цей рядок
        }

        Node* new_node = new Node{ m, nullptr };
        if (!head) head = tail = new_node;
        else {
            tail->next = new_node;
            tail = new_node;
        }
    }
    return head;
}

bool german_adult_without_synopsis(const Movie& m) {
    return m.rating == "18+" && m.studio_info.country == "Germany" && m.synopsis.empty();
}

Node* extract_all(Node*& head, bool(*pred)(const Movie&)) {
    Node* new_head = nullptr;
    Node* new_tail = nullptr;
    Node* current = head;
    Node* prev = nullptr;

    while (current) {
        if (pred(current->data)) {
            Node* to_extract = current;
            current = current->next;
            if (prev) prev->next = current;
            else head = current;

            to_extract->next = nullptr;
            if (!new_head) new_head = new_tail = to_extract;
            else {
                new_tail->next = to_extract;
                new_tail = to_extract;
            }
        }
        else {
            prev = current;
            current = current->next;
        }
    }
    return new_head;
}

void print_list(std::ostream& out_stream, const Node* head) {
    while (head) {
        out_stream << "Title: " << head->data.title
            << ", Year: " << head->data.release_year
            << ", Runtime: " << head->data.runtime_minutes << " min"
            << ", Rating: " << head->data.rating
            << ", Country: " << head->data.studio_info.country
            << ", Synopsis: " << (head->data.synopsis.empty() ? "(none)" : head->data.synopsis) << '\n';
        head = head->next;
    }
}

void write_to_tsv(std::ofstream& out_file, const Node* head) {
    out_file << "title\trelease_year\truntime_minutes\tsynopsis\trating\tstudio_name\twebsite\tcountry\n";
    while (head) {
        out_file << head->data.title << '\t'
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

void free_list(Node* head) {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8); // для правильного виводу UTF-8 в Windows

    std::ifstream movies_file("movies.tsv");
    std::ifstream studios_file("studios.tsv");
    if (!movies_file || !studios_file) {
        std::cerr << "Не вдалося відкрити один із файлів movies.tsv або studios.tsv" << std::endl;
        return 1;
    }

    Node* all_movies = read_from_tsv(movies_file, studios_file);
    movies_file.close();
    studios_file.close();

    std::cout << "\n=== ОРИГІНАЛЬНИЙ СПИСОК ===" << std::endl;
    print_list(std::cout, all_movies);

    Node* extracted = extract_all(all_movies, german_adult_without_synopsis);

    std::cout << "\n=== ВИДАЛЕНІ ЗАПИСИ ===" << std::endl;
    print_list(std::cout, extracted);

    std::cout << "\n=== СПИСОК ПІСЛЯ ВИДАЛЕННЯ ===" << std::endl;
    print_list(std::cout, all_movies);

    std::ofstream out_file("extracted_movies.tsv");
    if (!out_file) {
        std::cerr << "Помилка створення файлу extracted_movies.tsv" << std::endl;
        return 1;
    }
    write_to_tsv(out_file, extracted);
    out_file.close();

    free_list(all_movies);
    free_list(extracted);
    return 0;
}
