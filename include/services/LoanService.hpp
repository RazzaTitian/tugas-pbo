#pragma once

#include "models/Loan.hpp"
#include "repositories/CsvBookRepository.hpp"
#include "repositories/CsvLoanRepository.hpp"

#include <string>
#include <vector>

class LoanService {
private:
    CsvBookRepository& bookRepository_;
    CsvLoanRepository& loanRepository_;

public:
    LoanService(CsvBookRepository& bookRepository, CsvLoanRepository& loanRepository);

    bool borrowBook(
        int bookId,
        const std::string& memberId,
        const std::string& borrowDate,
        const std::string& dueDate
    );

    bool returnBook(
        int loanId,
        const std::string& returnDate
    );

    std::vector<Loan> listActiveLoans() const;
    std::vector<Loan> listLoanHistoryByMember(const std::string& memberId) const;
};