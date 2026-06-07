#include "MovieService.h"
#include "../DAL/DataSeeder.h"
#include <algorithm>
#include <chrono>
#include <random>
#include <iostream>

#define ASSET_PATH "assets/"

MovieService::MovieService(const std::string& dbPath) : dbRepo(dbPath)
{
    rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    initializeMovies();
}

void MovieService::initializeMovies()
{
    DataSeeder::SeedIfEmpty(dbRepo);
}

std::vector<Movie> MovieService::getRandomMovies(int count)
{
    std::vector<Movie> shuffled = dbRepo.getAllMovies();
    std::shuffle(shuffled.begin(), shuffled.end(), rng);

    if (shuffled.size() > (size_t)count) {
        shuffled.resize(count);
    }
    return shuffled;
}

std::vector<Movie> MovieService::getMoviesByGenre(const std::string& genre, int count)
{
    std::vector<Movie> filtered = dbRepo.getMoviesByGenre(genre);
    if (filtered.size() > (size_t)count) {
        filtered.resize(count);
    }
    return filtered;
}

std::vector<Movie> MovieService::getTopRatedMovies(int count)
{
    std::vector<Movie> sorted = dbRepo.getTopRatedMovies();
    if (sorted.size() > (size_t)count) {
        sorted.resize(count);
    }
    return sorted;
}

std::vector<Movie> MovieService::searchMovies(const std::string& query)
{
    if (query.empty()) return {};
    return dbRepo.searchMovies(query);
}

std::vector<Movie> MovieService::getAllMovies() const
{
    return const_cast<MovieRepository&>(dbRepo).getAllMovies();
}

Movie MovieService::getMovieById(int id)
{
    return dbRepo.getMovieById(id);
}

void MovieService::addMovie(const Movie& movie)
{
    dbRepo.addMovie(movie);
}

void MovieService::removeMovie(int id)
{
    dbRepo.removeMovie(id);
}