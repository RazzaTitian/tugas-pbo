#include "repositories/CsvAdminRepository.hpp"

#include <fstream>
#include <sstream>
#include <utility>
#include <vector>

CsvAdminRepository::CsvAdminRepository(std::string filePath)
    : filePath_(std::move(filePath)) {}

Admin CsvAdminRepository::fromCsvLine(const std::string &line) {
  std::stringstream ss(line);

  std::string username;
  std::string passwordHash;
  std::string displayName;

  std::getline(ss, username, ',');
  std::getline(ss, passwordHash, ',');
  std::getline(ss, displayName, ',');

  return Admin(username, passwordHash, displayName);
}

std::string CsvAdminRepository::toCsvLine(const Admin &admin) {
  std::stringstream ss;

  ss << admin.username() << "," << admin.passwordHash() << ","
     << admin.displayName();

  return ss.str();
}

bool CsvAdminRepository::save(const Admin &admin) {
  std::vector<Admin> admins = listAll();
  bool updated = false;

  for (Admin &existingAdmin : admins) {
    if (existingAdmin.username() == admin.username()) {
      existingAdmin = admin;
      updated = true;
      break;
    }
  }

  if (!updated) {
    admins.push_back(admin);
  }

  std::ofstream file(filePath_, std::ios::trunc);
  if (!file.is_open()) {
    return false;
  }

  for (const Admin &currentAdmin : admins) {
    file << toCsvLine(currentAdmin) << '\n';
  }

  return true;
}

std::optional<Admin>
CsvAdminRepository::findById(const std::string &username) const {
  std::vector<Admin> admins = listAll();

  for (const Admin &admin : admins) {
    if (admin.username() == username) {
      return admin;
    }
  }

  return std::nullopt;
}

std::vector<Admin> CsvAdminRepository::listAll() const {
  std::vector<Admin> admins;
  std::ifstream file(filePath_);

  if (!file.is_open()) {
    return admins;
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    admins.push_back(fromCsvLine(line));
  }

  return admins;
}

bool CsvAdminRepository::remove(const std::string &username) {
  std::vector<Admin> admins = listAll();
  std::vector<Admin> remainingAdmins;

  bool removed = false;

  for (const Admin &admin : admins) {
    if (admin.username() == username) {
      removed = true;
      continue;
    }

    remainingAdmins.push_back(admin);
  }

  if (!removed) {
    return false;
  }

  std::ofstream file(filePath_, std::ios::trunc);
  if (!file.is_open()) {
    return false;
  }

  for (const Admin &admin : remainingAdmins) {
    file << toCsvLine(admin) << '\n';
  }

  return true;
}