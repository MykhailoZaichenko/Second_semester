#ifndef MOVIE_H
#define MOVIE_H

#include <string>

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

#endif // MOVIE_H