#include "MovieService.h"
#include <algorithm>
#include <chrono>
#include <random>
#include <iostream>

#define ASSET_PATH "assets/"

MovieService::MovieService()
{
    rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    initializeMovies();
}

void MovieService::initializeMovies()
{
    availableMovies = {
        Movie(1, "It", ASSET_PATH "It.png", "Horror", 7.3f), 
        Movie(2, "Paddington", ASSET_PATH "Paddington.png", "Comedy", 7.8f),
        Movie(3, "Smile", ASSET_PATH "Smile.png", "Horror", 6.5f),
        Movie(4, "The Lion King", ASSET_PATH "LionKing.png", "Animation", 8.5f),
        Movie(5, "Zootopia 2", ASSET_PATH "Zootopia2.png", "Animation", 8.8f),
        Movie(6, "Titanic", ASSET_PATH "Titanic.png", "Romance", 9.0f),
        Movie(7, "The Conjuring", ASSET_PATH "theconjuring.png", "Horror", 8.6f),
        Movie(8, "Alvin and the Chipmunks", ASSET_PATH "Alvin.png", "Family", 7.4f),
    };
}

std::vector<Movie> MovieService::getRandomMovies(int count)
{
    std::vector<Movie> shuffled = availableMovies;
    std::shuffle(shuffled.begin(), shuffled.end(), rng);

    if (shuffled.size() > (size_t)count) {
        shuffled.resize(count);
    }
    return shuffled;
}

std::vector<Movie> MovieService::getMoviesByGenre(const std::string& genre, int count)
{
    std::vector<Movie> filtered;
    for (const auto& movie : availableMovies) {
        if (movie.getGenre() == genre) {
            filtered.push_back(movie);
        }
    }
    if (filtered.size() > (size_t)count) {
        filtered.resize(count);
    }
    return filtered;
}

std::vector<Movie> MovieService::getTopRatedMovies(int count)
{
    std::vector<Movie> sorted = availableMovies;
    std::sort(sorted.begin(), sorted.end(), [](const Movie& a, const Movie& b) {
        return a.getRating() > b.getRating();
        });

    if (sorted.size() > (size_t)count) {
        sorted.resize(count);
    }
    return sorted;
}

std::vector<Movie> MovieService::searchMovies(const std::string& query)
{
    std::vector<Movie> results;
    if (query.empty()) return results;

    std::string lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    for (const auto& movie : availableMovies) {
        std::string lowerTitle = movie.getTitle();
        std::transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);

        if (lowerTitle.find(lowerQuery) != std::string::npos) {
            results.push_back(movie);
        }
    }
    return results;
}

std::vector<Movie> MovieService::getAllMovies() const
{
    return availableMovies;
}

Movie MovieService::getMovieById(int id)
{
    for (const auto& movie : availableMovies) {
        if (movie.getId() == id) {
            return movie;
        }
    }
    return Movie(); 
}

void MovieService::addMovie(const Movie& movie)
{
    availableMovies.push_back(movie);
}

void MovieService::removeMovie(int id)
{
    availableMovies.erase(
        std::remove_if(availableMovies.begin(), availableMovies.end(),
            [id](const Movie& movie) { return movie.getId() == id; }),
        availableMovies.end()
    );
}