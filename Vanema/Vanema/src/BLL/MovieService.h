#pragma once
#include "Movie.h"
#include <vector>
#include <random>
#include <string>

class MovieService
{
private:
    std::vector<Movie> availableMovies;
    std::mt19937 rng;

    void initializeMovies();

public:
    MovieService();

    // Get random movies for display
    std::vector<Movie> getRandomMovies(int count);

    // Get movies by genre
    std::vector<Movie> getMoviesByGenre(const std::string& genre, int count);

    // Get top rated movies
    std::vector<Movie> getTopRatedMovies(int count);

    // Search movies by title
    std::vector<Movie> searchMovies(const std::string& query);

    // Get all movies
    std::vector<Movie> getAllMovies() const;

    // Get movie by ID
    Movie getMovieById(int id);

    // Add a new movie
    void addMovie(const Movie& movie);

    // Remove a movie
    void removeMovie(int id);
};