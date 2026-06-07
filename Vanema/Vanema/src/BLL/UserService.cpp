#include "UserService.h"
#include "../DAL/UserSeeder.h"

UserService::UserService(const std::string& dbPath) : userRepo(dbPath) {
    // Seed initial admin user if DB is fresh
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

    // Encrypt the plain-text password from the UI before storing it
    std::string securedHash = Crypto::hashPassword(password);

    // New accounts created via standard Sign-Up default to UserRole::STANDARD
    User newUser(0, username, email, securedHash, name, UserRole::STANDARD);
    return userRepo.addUser(newUser);
}

bool UserService::authenticateUser(const std::string& username, const std::string& password) {
    User user = userRepo.getUserByUsername(username);

    if (user.getId() == 0) {
        user = userRepo.getUserByEmail(username);
    }

    if (user.getId() == 0) return false; // Account not found

    // Secure Verification: Passwords cannot be decrypted.
    // Instead, bcrypt hashes the input password with the same salt and checks if the hashes match.
    return Crypto::verifyPassword(password, user.getPasswordHash());
}