#ifndef MOVIESERVICE_H
#define MOVIESERVICE_H

#include <vector>
#include <string>
#include "Movie.h"
#include "DAL/MovieRepository.h"

class MovieService {
private:
    MovieRepository repo;

public:
    MovieService(const std::string& dbPath);
    ~MovieService() = default;

    // Query methods
    std::vector<Movie> getMoviesByGenre(const std::string& genre);
    std::vector<Movie> getRandomMixForAll();
    std::vector<Movie> getRandomMovies(int count);
    Movie getMovieById(int id);
    std::vector<Movie> getAllMovies();

    // CRUD operations
    void addMovie(const Movie& movie);
    void deleteMovie(const std::string& title);
    void deleteMovie(int id);  // Overload for ID-based deletion

    // Reload method (for compatibility)
    void reloadMovies();
};

#endif