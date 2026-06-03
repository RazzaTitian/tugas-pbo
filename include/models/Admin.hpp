#pragma once

#include "models/User.hpp"

#include <iostream>
#include <string>

class Admin : public User {
private:
  std::string displayName_;

public:
  Admin();
  Admin(std::string username, std::string passwordHash,
        std::string displayName);

  const std::string &displayName() const;
  void setDisplayName(const std::string &displayName);

  std::string role() const override;

  bool operator==(const Admin &other) const;

  friend std::ostream &operator<<(std::ostream &os, const Admin &admin);
};