#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <SQLiteCpp/SQLiteCpp.h>
#include <vector>
#include <string>
#include "User.h"

class UserRepository {
private:
    SQLite::Database db;
    void createTable();

public:
    UserRepository(const std::string& dbPath);
    ~UserRepository() = default;

    bool addUser(const User& user);
    User getUserByUsername(const std::string& username);
    User getUserByEmail(const std::string& email);
    std::vector<User> getAllUsers();
};

#endif