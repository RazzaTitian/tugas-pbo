#include "models/Admin.hpp"

#include <utility>

Admin::Admin() : User(), displayName_("") {}

Admin::Admin(std::string username, std::string passwordHash,
             std::string displayName)
    : User(std::move(username), std::move(passwordHash)),
      displayName_(std::move(displayName)) {}

const std::string &Admin::displayName() const { return displayName_; }

void Admin::setDisplayName(const std::string &displayName) {
  displayName_ = displayName;
}

std::string Admin::role() const { return "admin"; }

bool Admin::operator==(const Admin &other) const {
  return username_ == other.username_;
}

std::ostream &operator<<(std::ostream &os, const Admin &admin) {
  os << "Admin{username=" << admin.username_ << ", displayName=\""
     << admin.displayName_ << "\"}";
  return os;
}