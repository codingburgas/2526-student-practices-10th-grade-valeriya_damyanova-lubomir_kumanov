#include "UserRepository.h"
#include <iostream>

UserRepository::UserRepository(const std::string& dbPath)
    : db(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)
{
    createTable();
}

void UserRepository::createTable() {
    try {
        db.exec("CREATE TABLE IF NOT EXISTS users ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "username TEXT NOT NULL UNIQUE, "
            "email TEXT NOT NULL UNIQUE, "
            "password TEXT NOT NULL, " 
            "name TEXT, "
            "role TEXT NOT NULL);"); 
    }
    catch (std::exception& e) {
        std::cerr << "SQL error creating users table: " << e.what() << std::endl;
    }
}

bool UserRepository::addUser(const User& user) {
    try {
        SQLite::Statement query(db, "INSERT INTO users (username, email, password, name, role) VALUES (?, ?, ?, ?, ?);");
        query.bind(1, user.getUsername());
        query.bind(2, user.getEmail());
        query.bind(3, user.getPasswordHash()); 
        query.bind(4, user.getName());
        query.bind(5, user.getRoleAsString());
        return (query.exec() > 0);
    }
    catch (std::exception& e) {
        std::cerr << "Registration Error: " << e.what() << std::endl;
        return false;
    }
}

// Helper internally mapping text rows back to User objects
User rowToUser(SQLite::Statement& query) {
    std::string roleStr = query.getColumn(5).getText();
    UserRole role = (roleStr == "Admin") ? UserRole::ADMIN : UserRole::STANDARD;

    return User(
        query.getColumn(0).getInt(),
        query.getColumn(1).getText(),
        query.getColumn(2).getText(),
        query.getColumn(3).getText(),
        query.getColumn(4).getText(),
        role
    );
}

User UserRepository::getUserByUsername(const std::string& username) {
    try { 
        SQLite::Statement query(db, "SELECT id, username, email, password, name, role FROM users WHERE username = ?");
        query.bind(1, username);

        if (query.executeStep()) {
            std::string roleStr = query.getColumn(5).getText();
            UserRole role = (roleStr == "Admin") ? UserRole::ADMIN : UserRole::STANDARD;

            return User(
                query.getColumn(0).getInt(),
                query.getColumn(1).getText(),
                query.getColumn(2).getText(),
                query.getColumn(3).getText(),
                query.getColumn(4).getText(),
                role
            );
        }
    }
    catch (std::exception& e) {
        std::cerr << "Error fetching user: " << e.what() << std::endl;
    }

    return User(0, "", "", "", "", UserRole::STANDARD);
}

User UserRepository::getUserByEmail(const std::string& email) {
    try {
       
        SQLite::Statement query(db, "SELECT id, username, email, password, name, role FROM users WHERE email = ?");
        query.bind(1, email);

        if (query.executeStep()) {
            std::string roleStr = query.getColumn(5).getText();
            UserRole role = (roleStr == "Admin") ? UserRole::ADMIN : UserRole::STANDARD;

            return User(
                query.getColumn(0).getInt(),
                query.getColumn(1).getText(),
                query.getColumn(2).getText(),
                query.getColumn(3).getText(),
                query.getColumn(4).getText(),
                role
            );
        }
    }
    catch (std::exception& e) {
        std::cerr << "Error fetching user by email: " << e.what() << std::endl;
    }

    return User(0, "", "", "", "", UserRole::STANDARD);
}

std::vector<User> UserRepository::getAllUsers() {
    std::vector<User> users;
    try {
       
        SQLite::Statement query(db, "SELECT id, username, email, password, name, role FROM users");

        while (query.executeStep()) {
            std::string roleStr = query.getColumn(5).getText(); 
            UserRole role = (roleStr == "Admin") ? UserRole::ADMIN : UserRole::STANDARD;

            users.emplace_back(
                query.getColumn(0).getInt(),         
                query.getColumn(1).getText(),        
                query.getColumn(2).getText(),        
                query.getColumn(3).getText(),        
                query.getColumn(4).getText(),        
                role                                 
            );
        }
    }
    catch (std::exception& e) {
        std::cerr << "Error fetching all users: " << e.what() << std::endl;
    }
    return users;
}