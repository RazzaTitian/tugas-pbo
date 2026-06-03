#include "models/Member.hpp"

#include <utility>

Member::Member() : User(), memberId_(""), name_(""), email_("") {}

Member::Member(std::string memberId, std::string username,
               std::string passwordHash, std::string name, std::string email)
    : User(std::move(username), std::move(passwordHash)),
      memberId_(std::move(memberId)), name_(std::move(name)),
      email_(std::move(email)) {}

const std::string &Member::memberId() const { return memberId_; }

const std::string &Member::name() const { return name_; }

const std::string &Member::email() const { return email_; }

void Member::setName(const std::string &name) { name_ = name; }

void Member::setEmail(const std::string &email) { email_ = email; }

std::string Member::role() const { return "member"; }

bool Member::operator==(const Member &other) const {
  return memberId_ == other.memberId_;
}

std::ostream &operator<<(std::ostream &os, const Member &member) {
  os << "Member{id=" << member.memberId_ << ", username=" << member.username_
     << ", name=\"" << member.name_ << "\", email=\"" << member.email_ << "\"}";
  return os;
}