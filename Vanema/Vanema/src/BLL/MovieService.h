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

    std::vector<Movie> getRandomMovies(int count);

    std::vector<Movie> getMoviesByGenre(const std::string& genre, int count);

    std::vector<Movie> getTopRatedMovies(int count);

    std::vector<Movie> searchMovies(const std::string& query);

    std::vector<Movie> getAllMovies() const;

    Movie getMovieById(int id);

    void addMovie(const Movie& movie);

    void removeMovie(int id);
};