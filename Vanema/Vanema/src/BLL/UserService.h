#ifndef USERSERVICE_H
#define USERSERVICE_H

#include <string>
#include "../DAL/bcrypt.h"
#include "../DAL/UserRepository.h"
#include "User.h"

class UserService {
private:
    UserRepository userRepo;

public:
    UserService(const std::string& dbPath = "movies.db");

    bool registerUser(const std::string& username, const std::string& email,
        const std::string& password, const std::string& name, std::string& outError);

    bool authenticateUser(const std::string& username, const std::string& password);
    User getUserDetails(const std::string& usernameOrEmail);
};

#endif