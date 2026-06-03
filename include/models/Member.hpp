#pragma once

#include "models/User.hpp"

#include <iostream>
#include <string>

class Member : public User {
private:
  std::string memberId_;
  std::string name_;
  std::string email_;

public:
  Member();
  Member(std::string memberId, std::string username, std::string passwordHash,
         std::string name, std::string email);

  const std::string &memberId() const;
  const std::string &name() const;
  const std::string &email() const;

  void setName(const std::string &name);
  void setEmail(const std::string &email);

  std::string role() const override;

  bool operator==(const Member &other) const;

  friend std::ostream &operator<<(std::ostream &os, const Member &member);
};