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
    Studio studioInfo;
};

struct Node {
    Movie data;
    Node* next;

    Node(const Movie& movie) : data(movie), next(nullptr) {}
};

// Структура для зберігання результату розбору рядка на токени
struct TokenResult {
    std::string token;
    size_t nextPos;
};

// Функція для отримання наступного токену з рядка
TokenResult getNextToken(const std::string& str, size_t start_pos, char delim) {
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

StudioNode* readStudios(std::ifstream& studiosFile) {
    StudioNode* head = nullptr;
    std::string line;

    // Пропускаємо заголовок файлу
    std::getline(studiosFile, line);

    while (std::getline(studiosFile, line)) {
        // Розбираємо рядок на токени
        size_t pos = 0;
        // Updated to match: studio_id	studio_name	country	website
        TokenResult idToken = getNextToken(line, pos, '\t');
        TokenResult nameToken = getNextToken(line, idToken.nextPos, '\t');
        TokenResult countryToken = getNextToken(line, nameToken.nextPos, '\t');
        TokenResult websiteToken = getNextToken(line, countryToken.nextPos, '\t');

        // Створюємо новий вузол та додаємо його до списку
        int id = std::stoi(idToken.token);
        Studio studio = { nameToken.token, websiteToken.token, countryToken.token };

        StudioNode* newNode = new StudioNode(id, studio);
        newNode->next = head;
        head = newNode;
    }
    return head;
}

void deleteStudios(StudioNode* head) {
    while (head != nullptr) {
        StudioNode* temp = head;
        head = head->next;
        delete temp;
    }
}

Studio* findStudio(StudioNode* studios, int id) {
    while (studios != nullptr) {
        if (studios->id == id) {
            return &studios->data;
        }
        studios = studios->next;
    }
    return nullptr;
}

Node* readFromTsv(std::ifstream& moviesFile, std::ifstream& studiosFile) {
    StudioNode* studios = readStudios(studiosFile);

    std::string line;
    Node* head = nullptr;
    Node* tail = nullptr;

    // Пропускаємо заголовок файлу фільмів
    std::getline(moviesFile, line);

    // Читаємо дані про фільми
    while (std::getline(moviesFile, line)) {
        // Розбираємо рядок на токени
        size_t pos = 0;
        // movie_id	title	release_year	runtime_minutes	synopsis	rating	studio_id
        TokenResult idToken = getNextToken(line, pos, '\t'); // movie_id (skip)
        TokenResult titleToken = getNextToken(line, idToken.nextPos, '\t');
        TokenResult yearToken = getNextToken(line, titleToken.nextPos, '\t');
        TokenResult runtimeToken = getNextToken(line, yearToken.nextPos, '\t');
        TokenResult synopsisToken = getNextToken(line, runtimeToken.nextPos, '\t');
        TokenResult ratingToken = getNextToken(line, synopsisToken.nextPos, '\t');
        TokenResult studioIdToken = getNextToken(line, ratingToken.nextPos, '\t');

        // Створюємо об'єкт фільму
        Movie movie;
        movie.title = titleToken.token;
        movie.release_year = std::stoi(yearToken.token);
        movie.runtime_minutes = std::stoi(runtimeToken.token);
        movie.synopsis = synopsisToken.token;
        movie.rating = ratingToken.token;

        int studio_id;
        try {
            studio_id = std::stoi(studioIdToken.token);
        }
        catch (...) {
            std::cerr << "Warning: Invalid studio ID \"" << studioIdToken.token
                << "\" for movie \"" << titleToken.token << "\"." << std::endl;
            studio_id = -1;
        }

        // Завжди намагаємось призначити студію (навіть якщо ID невалідний)
        Studio* studio = findStudio(studios, studio_id);
        if (studio != nullptr) {
            movie.studioInfo = *studio;
        }


        // Створюємо новий вузол та додаємо його до списку
        Node* newNode = new Node(movie);

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    // Звільняємо пам'ять списку студій
    deleteStudios(studios);
    return head;
}

// Функція для вилучення елементів, що задовольняють предикату
Node* extractMovies(Node*& head, bool(*pred)(const Movie&)) {
    Node* extractedHead = nullptr;
    Node* extractedTail = nullptr;
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

            if (extractedHead == nullptr) {
                extractedHead = current;
                extractedTail = current;
            }
            else {
                extractedTail->next = current;
                extractedTail = current;
            }

            current = next;
        }
        else {
            prev = current;
            current = current->next;
        }
    }

    return extractedHead;
}

void printList(std::ostream& out_stream, const Node* head) {
    if (head == nullptr) {
        out_stream << u8"List is empty.\n";
        return;
    }
    out_stream << std::left << std::setw(20) << "Title" << std::setw(8) << "Year" << std::setw(10) << "Runtime" << std::setw(30) << "Synopsis" << std::setw(10) << "Rating" << std::setw(20) << "Studio" << std::setw(15) << "Country" << std::setw(30) << "Website"  << std::endl;
    while (head != nullptr) {
        const Movie& movie = head->data;

        std::ostringstream runtimeStream;
        runtimeStream << movie.runtime_minutes << " min";

        out_stream << std::left << std::setw(20) << movie.title
            << std::setw(8) << movie.release_year
            << std::setw(12) << runtimeStream.str()
            << std::setw(30) << (movie.synopsis.empty() ? "(none)" : movie.synopsis)
            << std::setw(10) << movie.rating
            << std::setw(20) << movie.studioInfo.studio_name
            << std::setw(15) << movie.studioInfo.country
            << std::setw(30) << movie.studioInfo.website
            << std::endl;
        head = head->next;
    }
}

void writeToTsv(std::ofstream& outFile, const Node* head) {
    // Записуємо заголовок файлу
    outFile << "Title\tYear\tRuntime\tSynopsis\tRating\tStudio\tCountry\tWebsite\n";

    // Записуємо дані про кожен фільм
    while (head != nullptr) {
        const Movie& movie = head->data;
        outFile << movie.title << "\t"
            << movie.release_year << "\t"
            << movie.runtime_minutes << "\t"
            << (movie.synopsis.empty() ? "(none)" : movie.synopsis) << "\t"
            << movie.rating << "\t"
            << movie.studioInfo.studio_name << "\t"
            << movie.studioInfo.country << "\t"
            << movie.studioInfo.website << "\n";
        head = head->next;
    }
}

// Перевірка для фільтрації німецьких фільмів 18+ без опису
bool isGermanAdultWithoutSynopsis(const Movie& movie) {
    return movie.studioInfo.country == "Germany" && movie.rating == "18+" && movie.synopsis.empty();
}

// Функція для видалення списку фільмів та звільнення пам'яті
void deleteMovie(Node* head) {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    std::ifstream moviesFile("movies.tsv");
    std::ifstream studiosFile("studios.tsv");

    if (!moviesFile.is_open() || !studiosFile.is_open()) {
        std::cout << u8"Error: Unable to open input files" << std::endl;
        return 1;
    }

    Node* head = readFromTsv(moviesFile, studiosFile);

    std::cout << u8"\nInitial list of films:" << std::endl;
    printList(std::cout, head);

    Node* extracted = extractMovies(head, isGermanAdultWithoutSynopsis);

    std::cout << u8"\nList after removal:" << std::endl;
    printList(std::cout, head);

    std::cout << u8"\nRemoved films:" << std::endl;
    printList(std::cout, extracted);

    std::ofstream outFile("extracted_movies.tsv");
    if (outFile.is_open()) {
        writeToTsv(outFile, extracted);
    }

    deleteMovie(head);
    deleteMovie(extracted);

    moviesFile.close();
    studiosFile.close();
    outFile.close();

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
