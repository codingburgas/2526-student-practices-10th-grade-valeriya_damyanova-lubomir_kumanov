#include "MovieRepository.h"
#include <iostream>

MovieRepository::MovieRepository(const std::string& dbPath)
    : db(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) {
    createTable();
}

void MovieRepository::createTable() {
    try {
        db.exec("CREATE TABLE IF NOT EXISTS movies ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "title TEXT NOT NULL, "
            "poster_path TEXT, "
            "genre TEXT, "
            "rating REAL, "
            "description TEXT, "
            "duration INTEGER);");
    }
    catch (std::exception& e) {
        std::cerr << "SQL error creating table: " << e.what() << std::endl;
    }
}

std::vector<Movie> MovieRepository::getAllMovies() {
    std::vector<Movie> movies;
    try {
        SQLite::Statement query(db, "SELECT id, title, poster_path, genre, rating, description, duration FROM movies;");
        while (query.executeStep()) {
            movies.emplace_back(
                query.getColumn(0).getInt(),
                query.getColumn(1).getText(),
                query.getColumn(2).getText(),
                query.getColumn(3).getText(),
                static_cast<float>(query.getColumn(4).getDouble()),
                query.getColumn(5).getText(),
                query.getColumn(6).getInt()
            );
        }
    }
    catch (std::exception& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
    return movies;
}

Movie MovieRepository::getMovieById(int id) {
    try {
        SQLite::Statement query(db, "SELECT id, title, poster_path, genre, rating, description, duration FROM movies WHERE id = ?;");
        query.bind(1, id);
        if (query.executeStep()) {
            return Movie(
                query.getColumn(0).getInt(),
                query.getColumn(1).getText(),
                query.getColumn(2).getText(),
                query.getColumn(3).getText(),
                static_cast<float>(query.getColumn(4).getDouble()),
                query.getColumn(5).getText(),
                query.getColumn(6).getInt()
            );
        }
    }
    catch (std::exception& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
    return Movie();
}

std::vector<Movie> MovieRepository::getMoviesByGenre(const std::string& genre) {
    std::vector<Movie> movies;
    try {
        SQLite::Statement query(db, "SELECT id, title, poster_path, genre, rating, description, duration FROM movies WHERE genre = ?;");
        query.bind(1, genre);
        while (query.executeStep()) {
            movies.emplace_back(
                query.getColumn(0).getInt(),
                query.getColumn(1).getText(),
                query.getColumn(2).getText(),
                query.getColumn(3).getText(),
                static_cast<float>(query.getColumn(4).getDouble()),
                query.getColumn(5).getText(),
                query.getColumn(6).getInt()
            );
        }
    }
    catch (std::exception& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
    return movies;
}

std::vector<Movie> MovieRepository::getTopRatedMovies() {
    std::vector<Movie> movies;
    try {
        SQLite::Statement query(db, "SELECT id, title, poster_path, genre, rating, description, duration FROM movies ORDER BY rating DESC;");
        while (query.executeStep()) {
            movies.emplace_back(
                query.getColumn(0).getInt(),
                query.getColumn(1).getText(),
                query.getColumn(2).getText(),
                query.getColumn(3).getText(),
                static_cast<float>(query.getColumn(4).getDouble()),
                query.getColumn(5).getText(),
                query.getColumn(6).getInt()
            );
        }
    }
    catch (std::exception& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
    return movies;
}

std::vector<Movie> MovieRepository::searchMovies(const std::string& queryStr) {
    std::vector<Movie> movies;
    try {
        SQLite::Statement query(db, "SELECT id, title, poster_path, genre, rating, description, duration FROM movies WHERE title LIKE ?;");
        std::string wildQuery = "%" + queryStr + "%";
        query.bind(1, wildQuery);

        while (query.executeStep()) {
            movies.emplace_back(
                query.getColumn(0).getInt(),
                query.getColumn(1).getText(),
                query.getColumn(2).getText(),
                query.getColumn(3).getText(),
                static_cast<float>(query.getColumn(4).getDouble()),
                query.getColumn(5).getText(),
                query.getColumn(6).getInt()
            );
        }
    }
    catch (std::exception& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
    return movies;
}

std::vector<Movie> MovieRepository::getRandomMoviePerGenre() {
    std::vector<Movie> movies;
    try {
        SQLite::Statement query(db,
            "SELECT id, title, poster_path, genre, rating, description, duration FROM ("
            "SELECT id, title, poster_path, genre, rating, description, duration FROM movies ORDER BY RANDOM()"
            ") GROUP BY genre;");

        while (query.executeStep()) {
            movies.emplace_back(
                query.getColumn(0).getInt(),
                query.getColumn(1).getText(),
                query.getColumn(2).getText(),
                query.getColumn(3).getText(),
                static_cast<float>(query.getColumn(4).getDouble()),
                query.getColumn(5).getText(),
                query.getColumn(6).getInt()
            );
        }
    }
    catch (std::exception& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
    return movies;
}

bool MovieRepository::addMovie(const Movie& movie) {
    try {
        SQLite::Statement query(db, "INSERT INTO movies (title, poster_path, genre, rating, description, duration) VALUES (?, ?, ?, ?, ?, ?);");
        query.bind(1, movie.getTitle());
        query.bind(2, movie.getPosterPath());
        query.bind(3, movie.getGenre());
        query.bind(4, movie.getRating());
        query.bind(5, movie.getDescription());
        query.bind(6, movie.getDuration());

        return (query.exec() > 0);
    }
    catch (std::exception& e) {
        std::cerr << "Database error adding movie: " << e.what() << std::endl;
        return false;
    }
}

// NEW: Delete movie by ID
bool MovieRepository::deleteMovieById(int id) {
    try {
        SQLite::Statement query(db, "DELETE FROM movies WHERE id = ?;");
        query.bind(1, id);
        int rowsAffected = query.exec();
        if (rowsAffected > 0) {
            std::cout << "Movie with ID " << id << " deleted successfully." << std::endl;
            return true;
        }
        else {
            std::cout << "No movie found with ID " << id << std::endl;
            return false;
        }
    }
    catch (std::exception& e) {
        std::cerr << "Database error deleting movie by ID: " << e.what() << std::endl;
        return false;
    }
}

// NEW: Delete movie by Title
bool MovieRepository::deleteMovieByTitle(const std::string& title) {
    try {
        SQLite::Statement query(db, "DELETE FROM movies WHERE title = ?;");
        query.bind(1, title);
        int rowsAffected = query.exec();
        if (rowsAffected > 0) {
            std::cout << "Movie with title '" << title << "' deleted successfully." << std::endl;
            return true;
        }
        else {
            std::cout << "No movie found with title '" << title << "'" << std::endl;
            return false;
        }
    }
    catch (std::exception& e) {
        std::cerr << "Database error deleting movie by title: " << e.what() << std::endl;
        return false;
    }
}

// NEW: Update movie
bool MovieRepository::updateMovie(const Movie& movie) {
    try {
        SQLite::Statement query(db,
            "UPDATE movies SET title = ?, poster_path = ?, genre = ?, rating = ?, description = ?, duration = ? WHERE id = ?;");
        query.bind(1, movie.getTitle());
        query.bind(2, movie.getPosterPath());
        query.bind(3, movie.getGenre());
        query.bind(4, movie.getRating());
        query.bind(5, movie.getDescription());
        query.bind(6, movie.getDuration());
        query.bind(7, movie.getId());

        int rowsAffected = query.exec();
        if (rowsAffected > 0) {
            std::cout << "Movie with ID " << movie.getId() << " updated successfully." << std::endl;
            return true;
        }
        else {
            std::cout << "No movie found with ID " << movie.getId() << std::endl;
            return false;
        }
    }
    catch (std::exception& e) {
        std::cerr << "Database error updating movie: " << e.what() << std::endl;
        return false;
    }
}