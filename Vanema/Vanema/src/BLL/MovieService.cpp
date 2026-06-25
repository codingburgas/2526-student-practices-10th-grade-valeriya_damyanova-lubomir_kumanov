#include "MovieService.h"
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>

MovieService::MovieService(const std::string& dbPath) : repo(dbPath) {
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned int>(time(nullptr)));
        seeded = true;
    }
}

std::vector<Movie> MovieService::getMoviesByGenre(const std::string& genre) {
    return repo.getMoviesByGenre(genre);
}

std::vector<Movie> MovieService::getRandomMixForAll() {
    std::vector<Movie> mixResult;

    std::vector<std::string> activeGenres = {
        "Action", "Adventure", "Animation", "Comedy",
        "Drama", "Horror", "Sci-Fi", "Romance", "Family"
    };

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(activeGenres.begin(), activeGenres.end(), g);

    for (int i = 0; i < 8 && i < (int)activeGenres.size(); i++) {
        std::vector<Movie> pool = repo.getMoviesByGenre(activeGenres[i]);
        if (!pool.empty()) {
            int randomIdx = rand() % pool.size();
            mixResult.push_back(pool[randomIdx]);
        }
    }

    return mixResult;
}

std::vector<Movie> MovieService::getRandomMovies(int count) {
    std::vector<Movie> allMovies = repo.getAllMovies();
    std::vector<Movie> randomSelection;

    if (allMovies.empty()) return randomSelection;

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(allMovies.begin(), allMovies.end(), g);

    for (int i = 0; i < count && i < (int)allMovies.size(); i++) {
        randomSelection.push_back(allMovies[i]);
    }

    return randomSelection;
}

Movie MovieService::getMovieById(int id) {
    return repo.getMovieById(id);
}

std::vector<Movie> MovieService::getAllMovies() {
    return repo.getAllMovies();
}

// FIXED: Actually delete from database
void MovieService::deleteMovie(const std::string& title) {
    std::cout << "Deleting movie with title: " << title << std::endl;

    // Get all movies to find the one with matching title
    std::vector<Movie> allMovies = repo.getAllMovies();
    for (const auto& movie : allMovies) {
        if (movie.getTitle() == title) {
            // Delete by ID from the database
            bool success = repo.deleteMovieById(movie.getId());
            if (success) {
                std::cout << "Movie '" << title << "' deleted successfully." << std::endl;
            }
            else {
                std::cout << "Failed to delete movie '" << title << "'" << std::endl;
            }
            return;
        }
    }
    std::cout << "Movie not found: " << title << std::endl;
}

// Delete by ID
void MovieService::deleteMovie(int id) {
    std::cout << "Deleting movie with ID: " << id << std::endl;
    bool success = repo.deleteMovieById(id);
    if (success) {
        std::cout << "Movie with ID " << id << " deleted successfully." << std::endl;
    }
    else {
        std::cout << "Failed to delete movie with ID " << id << std::endl;
    }
}

// Reload movies (just a placeholder - database queries are always fresh)
void MovieService::reloadMovies() {
    std::cout << "Reloading movies from database..." << std::endl;
    // Nothing to do - the repository always queries the database directly
    // This method exists for compatibility with Films.cpp
}

void MovieService::addMovie(const Movie& movie) {
    repo.addMovie(movie);
}