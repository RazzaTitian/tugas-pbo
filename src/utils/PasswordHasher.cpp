#include "utils/PasswordHasher.hpp"

#include <functional>
#include <sstream>

std::string PasswordHasher::hash(const std::string& password) {
    std::hash<std::string> hasher;

    std::stringstream ss;
    ss << hasher(password);

    return ss.str();
}