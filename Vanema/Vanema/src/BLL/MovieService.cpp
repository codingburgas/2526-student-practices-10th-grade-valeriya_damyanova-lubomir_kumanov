#include "MovieService.h"
#include <algorithm>
#include <random>
#include <ctime>

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

    for (int i = 0; i < 8; i++) {
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

    for (int i = 0; i < count && i < allMovies.size(); i++) {
        randomSelection.push_back(allMovies[i]);
    }

    return randomSelection;
}

Movie MovieService::getMovieById(int id) {
    return repo.getMovieById(id);
}

void MovieService::deleteMovie(const std::string& title) {
    std::cout << "Successfully intercepted deletion request for: " << title << std::endl;
}