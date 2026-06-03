#include "Movie.h"

Movie::Movie()
    : id(0), title(""), posterPath(""), genre(""), rating(0.0f)
{}

Movie::Movie(int id, const std::string& title, const std::string& posterPath,
    const std::string& genre, float rating)
    : id(id), title(title), posterPath(posterPath), genre(genre), rating(rating)
{}

int Movie::getId() const { return id; }
std::string Movie::getTitle() const { return title; }
std::string Movie::getPosterPath() const { return posterPath; }
std::string Movie::getGenre() const { return genre; }
float Movie::getRating() const { return rating; }

void Movie::setId(int id) { this->id = id; }
void Movie::setTitle(const std::string& title) { this->title = title; }
void Movie::setPosterPath(const std::string& posterPath) { this->posterPath = posterPath; }
void Movie::setGenre(const std::string& genre) { this->genre = genre; }
void Movie::setRating(float rating) { this->rating = rating; }