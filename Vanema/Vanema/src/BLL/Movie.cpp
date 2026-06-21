#include "Movie.h"

Movie::Movie() : id(0), rating(0.0f), duration(0) {}

Movie::Movie(int id, const std::string& title, const std::string& posterPath,
    const std::string& genre, float rating, const std::string& description, int duration)
    : id(id), title(title), posterPath(posterPath), genre(genre), rating(rating),
    description(description), duration(duration) {}

int Movie::getId() const { return id; }
std::string Movie::getTitle() const { return title; }
std::string Movie::getPosterPath() const { return posterPath; }
std::string Movie::getGenre() const { return genre; }
float Movie::getRating() const { return rating; }
std::string Movie::getDescription() const { return description; }
int Movie::getDuration() const { return duration; }

void Movie::setId(int id) { this->id = id; }
void Movie::setTitle(const std::string& title) { this->title = title; }
void Movie::setPosterPath(const std::string& posterPath) { this->posterPath = posterPath; }
void Movie::setGenre(const std::string& genre) { this->genre = genre; }
void Movie::setRating(float rating) { this->rating = rating; }
void Movie::setDescription(const std::string& description) { this->description = description; }
void Movie::setDuration(int duration) { this->duration = duration; }