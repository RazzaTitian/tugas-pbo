#pragma once

#include "models/Admin.hpp"
#include "repositories/Repository.hpp"

#include <optional>
#include <string>
#include <vector>

class CsvAdminRepository : public Repository<Admin, std::string> {
private:
  std::string filePath_;

  static Admin fromCsvLine(const std::string &line);
  static std::string toCsvLine(const Admin &admin);

public:
  explicit CsvAdminRepository(std::string filePath);

  bool save(const Admin &admin) override;
  std::optional<Admin> findById(const std::string &username) const override;
  std::vector<Admin> listAll() const override;
  bool remove(const std::string &username) override;
};