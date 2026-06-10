#ifndef MOVIEREPOSITORY_H
#define MOVIEREPOSITORY_H

#include <SQLiteCpp/SQLiteCpp.h>
#include <vector>
#include <string>
#include "Movie.h"

class MovieRepository {
private:
    SQLite::Database db;
    void createTable();

public:
    MovieRepository(const std::string& dbPath);
    ~MovieRepository() = default;

    std::vector<Movie> getAllMovies();
    Movie getMovieById(int id);
    bool addMovie(const Movie& movie);
    bool removeMovie(int id);
    std::vector<Movie> getMoviesByGenre(const std::string& genre);
    std::vector<Movie> getTopRatedMovies();
    std::vector<Movie> searchMovies(const std::string& query);
    std::vector<Movie> getRandomMoviePerGenre();
};

#endif