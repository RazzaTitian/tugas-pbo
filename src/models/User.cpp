#include "models/User.hpp"

#include <utility>

User::User()
    : username_(""), passwordHash_("") {}

User::User(std::string username, std::string passwordHash)
    : username_(std::move(username)), passwordHash_(std::move(passwordHash)) {}

const std::string& User::username() const {
    return username_;
}

const std::string& User::passwordHash() const {
    return passwordHash_;
}

void User::setUsername(const std::string& username) {
    username_ = username;
}

void User::setPasswordHash(const std::string& passwordHash) {
    passwordHash_ = passwordHash;
}

bool User::login(const std::string& username, const std::string& passwordHash) const {
    return username_ == username && passwordHash_ == passwordHash;
}