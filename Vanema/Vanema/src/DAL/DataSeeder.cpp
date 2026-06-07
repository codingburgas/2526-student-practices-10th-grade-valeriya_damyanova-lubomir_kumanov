#include "DataSeeder.h"
#include <vector>

#define ASSET_PATH "assets/"

void DataSeeder::SeedIfEmpty(MovieRepository& repo)
{
    // 1. Check if data already exists to avoid duplicate seeding
    if (!repo.getAllMovies().empty()) {
        return;
    }

    // 2. Define your default catalog
    std::vector<Movie> defaultMovies = {
        Movie(0, "It", ASSET_PATH "It.png", "Horror", 7.3f),
        Movie(0, "Paddington", ASSET_PATH "Paddington.png", "Comedy", 7.8f),
        Movie(0, "Smile", ASSET_PATH "Smile.png", "Horror", 6.5f),
        Movie(0, "The Lion King", ASSET_PATH "LionKing.png", "Animation", 8.5f),
        Movie(0, "Zootopia 2", ASSET_PATH "Zootopia2.png", "Animation", 8.8f),
        Movie(0, "Titanic", ASSET_PATH "Titanic.png", "Romance", 9.0f),
        Movie(0, "The Conjuring", ASSET_PATH "theconjuring.png", "Horror", 8.6f),
        Movie(0, "Alvin and the Chipmunks", ASSET_PATH "Alvin.png", "Family", 7.4f)
    };

    // 3. Optional but highly recommended: Wrap bulk inserts in a transaction 
    // This prevents SQLite from writing to the disk 8 separate times.
    // (If your MovieRepository exposes a raw sqlite3 handle or a transaction method, use it here)

    for (const auto& movie : defaultMovies) {
        repo.addMovie(movie);
    }
}