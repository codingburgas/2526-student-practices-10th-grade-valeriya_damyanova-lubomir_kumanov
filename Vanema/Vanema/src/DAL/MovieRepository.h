#ifndef MOVIEREPOSITORY_H
#define MOVIEREPOSITORY_H

#include "Movie.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <vector>
#include <string>

class MovieRepository {
private:
    SQLite::Database db;

    void createTable();

public:
    MovieRepository(const std::string& dbPath);
    ~MovieRepository() = default;

    // Query methods
    std::vector<Movie> getAllMovies();
    Movie getMovieById(int id);
    std::vector<Movie> getMoviesByGenre(const std::string& genre);
    std::vector<Movie> getTopRatedMovies();
    std::vector<Movie> searchMovies(const std::string& queryStr);
    std::vector<Movie> getRandomMoviePerGenre();

    // CRUD operations
    bool addMovie(const Movie& movie);
    bool deleteMovieById(int id);
    bool deleteMovieByTitle(const std::string& title);
    bool updateMovie(const Movie& movie);
};

#endif