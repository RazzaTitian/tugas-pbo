#pragma once

#include "models/Admin.hpp"
#include "repositories/CsvAdminRepository.hpp"

#include <optional>
#include <string>

class AdminService {
private:
  CsvAdminRepository &adminRepository_;

public:
  explicit AdminService(CsvAdminRepository &adminRepository);

  bool authenticate(const std::string &username,
                    const std::string &password) const;

  bool createAdmin(const Admin &admin);

  std::optional<Admin> findAdmin(const std::string &username) const;
};