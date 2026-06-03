#pragma once

#include "models/Member.hpp"
#include "repositories/Repository.hpp"

#include <optional>
#include <string>
#include <vector>

class CsvMemberRepository : public Repository<Member, std::string> {
private:
  std::string filePath_;

  static Member fromCsvLine(const std::string &line);
  static std::string toCsvLine(const Member &member);

public:
  explicit CsvMemberRepository(std::string filePath);

  bool save(const Member &member) override;
  std::optional<Member> findById(const std::string &id) const override;
  std::vector<Member> listAll() const override;
  bool remove(const std::string &id) override;
};