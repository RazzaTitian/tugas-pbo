#pragma once

#include "models/Loan.hpp"
#include "repositories/Repository.hpp"

#include <optional>
#include <string>
#include <vector>

class CsvLoanRepository : public Repository<Loan, int> {
private:
    std::string filePath_;

    static Loan fromCsvLine(const std::string& line);
    static std::string toCsvLine(const Loan& loan);

public:
    explicit CsvLoanRepository(std::string filePath);

    bool save(const Loan& loan) override;
    std::optional<Loan> findById(const int& id) const override;
    std::vector<Loan> listAll() const override;
    bool remove(const int& id) override;

    std::vector<Loan> listActive() const;
    std::vector<Loan> listByMemberId(const std::string& memberId) const;
    std::vector<Loan> listOverdue(const std::string& today) const;

    int nextId() const;
};