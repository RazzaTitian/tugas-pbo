#pragma once

#include <string>

class User {
protected:
  std::string username_;
  std::string passwordHash_;

public:
  User();
  User(std::string username, std::string passwordHash);
  virtual ~User() = default;

  const std::string &username() const;
  const std::string &passwordHash() const;

  void setUsername(const std::string &username);
  void setPasswordHash(const std::string &passwordHash);

  bool login(const std::string &username,
             const std::string &passwordHash) const;

  virtual std::string role() const = 0;
};