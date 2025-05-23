#include "utils.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<Studio> read_studios_from_file(const std::string& filename) {
    std::vector<Studio> studios;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return studios;
    }

    std::string line;
    while (getline(file, line)) {
        std::istringstream iss(line);
        Studio s;
        getline(iss, s.studio_name, '\t');
        getline(iss, s.website, '\t');
        getline(iss, s.country, '\t');
        studios.push_back(s);
    }

    return studios;
}

Node* read_movies_from_file(const std::string& filename, const std::vector<Studio>& studios) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return nullptr;
    }

    Node* head = nullptr;
    Node* tail = nullptr;
    std::string line;
    while (getline(file, line)) {
        std::istringstream iss(line);
        Movie m;
        std::string studio_name;

        getline(iss, m.title, '\t');
        iss >> m.release_year;
        iss.ignore();
        iss >> m.runtime_minutes;
        iss.ignore();
        getline(iss, m.synopsis, '\t');
        getline(iss, m.rating, '\t');
        getline(iss, studio_name);

        for (const auto& s : studios) {
            if (s.studio_name == studio_name) {
                m.studio_info = s;
                break;
            }
        }

        Node* node = new Node{ m, nullptr };
        if (!head) head = tail = node;
        else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

void print_movies(Node* head) {
    Node* current = head;
    while (current) {
        const Movie& m = current->data;
        std::cout << "Title: " << m.title << "\n"
                  << "Year: " << m.release_year << ", Runtime: " << m.runtime_minutes << " min\n"
                  << "Rating: " << m.rating << "\n"
                  << "Studio: " << m.studio_info.studio_name << " (" << m.studio_info.country << ")\n"
                  << "Website: " << m.studio_info.website << "\n"
                  << "Synopsis: " << m.synopsis << "\n"
                  << std::string(40, '-') << std::endl;
        current = current->next;
    }
}

void free_list(Node*& head) {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}