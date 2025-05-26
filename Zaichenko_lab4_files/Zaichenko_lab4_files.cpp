#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>
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
        // Updated to match: studio_id	studio_name	country	website
        TokenResult id_token = get_next_token(line, pos, '\t');
        TokenResult name_token = get_next_token(line, id_token.next_pos, '\t');
        TokenResult country_token = get_next_token(line, name_token.next_pos, '\t');
        TokenResult website_token = get_next_token(line, country_token.next_pos, '\t');

        //TokenResult id_token = get_next_token(line, pos, '\t');
        //TokenResult name_token = get_next_token(line, id_token.next_pos, '\t');
        //TokenResult website_token = get_next_token(line, name_token.next_pos, '\t');
        //TokenResult country_token = get_next_token(line, website_token.next_pos, '\t');

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
        // movie_id	title	release_year	runtime_minutes	synopsis	rating	studio_id
        TokenResult id_token = get_next_token(line, pos, '\t'); // movie_id (skip)
        TokenResult title_token = get_next_token(line, id_token.next_pos, '\t');
        TokenResult year_token = get_next_token(line, title_token.next_pos, '\t');
        TokenResult runtime_token = get_next_token(line, year_token.next_pos, '\t');
        TokenResult synopsis_token = get_next_token(line, runtime_token.next_pos, '\t');
        TokenResult rating_token = get_next_token(line, synopsis_token.next_pos, '\t');
        TokenResult studio_id_token = get_next_token(line, rating_token.next_pos, '\t');

        //TokenResult id_token = get_next_token(line, pos, '\t');
        //TokenResult title_token = get_next_token(line, id_token.next_pos, '\t');
        //TokenResult year_token = get_next_token(line, title_token.next_pos, '\t');
        //TokenResult runtime_token = get_next_token(line, year_token.next_pos, '\t');
        //TokenResult synopsis_token = get_next_token(line, runtime_token.next_pos, '\t');
        //TokenResult rating_token = get_next_token(line, synopsis_token.next_pos, '\t');
        //TokenResult studio_id_token = get_next_token(line, rating_token.next_pos, '\t');

        // Створюємо об'єкт фільму
        Movie movie;
        movie.title = title_token.token;
        movie.release_year = std::stoi(year_token.token);
        movie.runtime_minutes = std::stoi(runtime_token.token);
        movie.synopsis = synopsis_token.token;
        movie.rating = rating_token.token;

        int studio_id;
        try {
            studio_id = std::stoi(studio_id_token.token);
        }
        catch (...) {
            std::cerr << "Warning: Invalid studio ID \"" << studio_id_token.token
                << "\" for movie \"" << title_token.token << "\"." << std::endl;
            studio_id = -1;
        }

        // Завжди намагаємось призначити студію (навіть якщо ID невалідний)
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

// Предикат для фільтрації німецьких фільмів 18+ без опису
bool is_german_adult_without_synopsis(const Movie& movie) {
    return movie.studio_info.country == "Germany" && movie.rating == "18+" && movie.synopsis.empty();
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
        out_stream << u8"List is empty.\n";
        return;
    }
    out_stream << std::left << std::setw(20) << "Title" << std::setw(8) << "Year" << std::setw(10) << "Runtime" << std::setw(30) << "Synopsis" << std::setw(10) << "Rating" << std::setw(20) << "Studio" << std::setw(15) << "Country" << std::setw(30) << "Website"  << std::endl;
    while (head != nullptr) {
        const Movie& movie = head->data;

        std::ostringstream runtime_stream;
        runtime_stream << movie.runtime_minutes << " min";

        out_stream << std::left << std::setw(20) << movie.title
            << std::setw(8) << movie.release_year
            << std::setw(12) << runtime_stream.str()
            << std::setw(30) << (movie.synopsis.empty() ? "(none)" : movie.synopsis)
            << std::setw(10) << movie.rating
            << std::setw(20) << movie.studio_info.studio_name
            << std::setw(15) << movie.studio_info.country
            << std::setw(30) << movie.studio_info.website
            << std::endl;
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
            << (movie.synopsis.empty() ? "(none)" : movie.synopsis) << "\t"
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
        std::cout << u8"Error: Unable to open input files" << std::endl;
        return 1;
    }

    Node* head = read_from_tsv(movies_file, studios_file);

    std::cout << u8"\nInitial list of films:" << std::endl;
    print_list(std::cout, head);

    Node* extracted = extract_all(head, is_german_adult_without_synopsis);

    std::cout << u8"\nList after removal:" << std::endl;
    print_list(std::cout, head);

    std::cout << u8"\nRemoved films:" << std::endl;
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

// TESTS
// 
// test1
// Empty files
// Input files:
// studios.tsv: only header
// movies.tsv: only header
// Console output:
// Initial list of films:
// List is empty.
// List after removal:
// List is empty.
// Removed films:
// List is empty.
// Output file extracted_movies.tsv: only header row

// test2
// One German 18+ movie with no synopsis
// Input data:
// studios.tsv:
// studio_id	studio_name	country	website
// 1	UFA	Germany	https://ufa.de
//
// movies.tsv:
// movie_id	title	release_year	runtime_minutes	synopsis	rating	studio_id
// 1	Adult Film	2020	95		18+	1
//
// Expected output:
// Initial list: contains "Adult Film"
// List after removal: List is empty
// Removed films: contains "Adult Film"
// Output file extracted_movies.tsv: contains "Adult Film"

// test3
// German 18+ movie with synopsis
// Input:
// studios.tsv:
// studio_id	studio_name	country	website
// 1	UFA	Germany	https://ufa.de
//
// movies.tsv:
// movie_id	title	release_year	runtime_minutes	synopsis	rating	studio_id
// 1	Adult Berlin	2018	100	A dramatic adult story.	18+	1
//
// Output:
// Initial list: contains "Adult Berlin"
// Removed films: List is empty
// Remaining list: contains "Adult Berlin"
// Output file: only header

// test4
// Mixed data with multiple studios
// studios.tsv:
// studio_id	studio_name	country	website
// 1	Studio Alpha	USA	https://alpha.com
// 2	Studio Beta	Germany	https://beta.de
// 3	UFA	Germany	https://ufa.de
//
// movies.tsv:
// movie_id	title	release_year	runtime_minutes	synopsis	rating	studio_id
// 1	Child Movie	2010	90	Magic	0+	1
// 2	German 18+	2011	88		18+	2
// 3	Other Movie	2015	100	Plot	16+	3
// 4	No Synopsis	2018	90		18+	3
//
// Output:
// Initial list: contains "Child Movie", "German 18+", "Other Movie", "No Synopsis"
// Removed films: contains "German 18+", "No Synopsis"
// Remaining list: contains "Child Movie", "Other Movie"
// extracted_movies.tsv: contains "German 18+", "No Synopsis"

// test5
// Non-German 18+ movie with empty synopsis
// studios.tsv:
// studio_id	studio_name	country	website
// 1	Studio Global	UK	https://global.com
//
// movies.tsv:
// movie_id	title	release_year	runtime_minutes	synopsis	rating	studio_id
// 1	No synopsis Adult	2022	90		18+	1
//
// Expected:
// Initial list: contains "No synopsis Adult"
// Removed films: List is empty
// Remaining list: contains "No synopsis Adult"
// extracted_movies.tsv: only header

// test6
// German movie with rating not equal to 18+
// studios.tsv:
// studio_id	studio_name	country	website
// 1	UFA	Germany	https://ufa.de
//
// movies.tsv:
// movie_id	title	release_year	runtime_minutes	synopsis	rating	studio_id
// 1	Serious Drama	2021	90	Very emotional.	16+	1
//
// Expected:
// Initial list: contains "Serious Drama"
// Removed films: List is empty
// Remaining list: contains "Serious Drama"
// extracted_movies.tsv: only header

// test7
// German 18+ movie with whitespace-only synopsis
// studios.tsv:
// studio_id	studio_name	country	website
// 1	UFA	Germany	https://ufa.de
//
// movies.tsv:
// movie_id	title	release_year	runtime_minutes	synopsis	rating	studio_id
// 1	Whitespace Movie	2020	91	 	18+	1
//
// Expected:
// Initial list: contains "Whitespace Movie"
// Removed films: List is empty
// Remaining list: contains "Whitespace Movie"
// extracted_movies.tsv: only header

// test8
// German 18+ movie with missing studio (invalid studio ID)
// studios.tsv:
// studio_id	studio_name	country	website
// 1	UFA	Germany	https://ufa.de
//
// movies.tsv:
// movie_id	title	release_year	runtime_minutes	synopsis	rating	studio_id
// 1	Orphan Movie	2019	87		18+	99
//
// Expected:
// Warning printed: Studio ID 99 not found
// Initial list: contains "Orphan Movie" with unknown studio
// Removed films: List is empty
// Remaining list: contains "Orphan Movie"
// extracted_movies.tsv: only header

// test9
// Invalid studio ID (non-numeric)
// studios.tsv:
// studio_id	studio_name	country	website
// 1	UFA	Germany	https://ufa.de
//
// movies.tsv:
// movie_id	title	release_year	runtime_minutes	synopsis	rating	studio_id
// 8	Parasite	2019	132	Greed and class discrimination threaten the newly formed symbiotic relationship between the wealthy Park family and the destitute Kim clan.	R	(South Korean Studio)
//
// Expected:
// Warning printed: Invalid studio ID "(South Korean Studio)" for movie "Parasite"
// Initial list: contains "Parasite" with unknown studio
// Removed films: List is empty
// Remaining list: contains "Parasite"
// extracted_movies.tsv: only header
