#include "UserService.h"
#include "../DAL/UserSeeder.h"

UserService::UserService(const std::string& dbPath) : userRepo(dbPath) {
    UserSeeder::SeedIfEmpty(userRepo);
}

bool UserService::registerUser(const std::string& username, const std::string& email,
    const std::string& password, const std::string& name, std::string& outError)
{
    if (userRepo.getUserByUsername(username).getId() != 0) {
        outError = "Username is already taken!";
        return false;
    }

    if (userRepo.getUserByEmail(email).getId() != 0) {
        outError = "Email is already registered!";
        return false;
    }

    std::string securedHash = Crypto::hashPassword(password);

    User newUser(0, username, email, securedHash, name, UserRole::STANDARD);
    return userRepo.addUser(newUser);
}

bool UserService::authenticateUser(const std::string& username, const std::string& password) {
    User user = userRepo.getUserByUsername(username);

    if (user.getId() == 0) {
        user = userRepo.getUserByEmail(username);
    }

    if (user.getId() == 0) return false; 
    return Crypto::verifyPassword(password, user.getPasswordHash());
}
User UserService::getUserDetails(const std::string& usernameOrEmail) {
    User user = userRepo.getUserByUsername(usernameOrEmail);
    if (user.getId() == 0) {
        user = userRepo.getUserByEmail(usernameOrEmail);
    }
    return user;
}
