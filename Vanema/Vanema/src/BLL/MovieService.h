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

    std::vector<Movie> getMoviesByGenre(const std::string& genre);
    std::vector<Movie> getRandomMixForAll(); 

    std::vector<Movie> getRandomMovies(int count);
    Movie getMovieById(int id);
};

#endif