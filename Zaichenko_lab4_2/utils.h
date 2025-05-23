#ifndef UTILS_H
#define UTILS_H

#include "movie.h"
#include <vector>
#include <string>

std::vector<Studio> read_studios_from_file(const std::string& filename);
Node* read_movies_from_file(const std::string& filename, const std::vector<Studio>& studios);
void print_movies(Node* head);
void free_list(Node*& head);

#endif // UTILS_H