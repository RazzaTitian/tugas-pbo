#pragma once

#include "models/Loan.hpp"
#include "repositories/CsvBookRepository.hpp"
#include "repositories/CsvLoanRepository.hpp"
#include "repositories/CsvMemberRepository.hpp"
#include "repositories/CsvReservationRepository.hpp"

#include <string>
#include <vector>

class LoanService {
private:
  CsvBookRepository &bookRepository_;
  CsvLoanRepository &loanRepository_;
  CsvMemberRepository &memberRepository_;
  CsvReservationRepository &reservationRepository_;

public:
  LoanService(CsvBookRepository &bookRepository,
              CsvLoanRepository &loanRepository,
              CsvMemberRepository &memberRepository,
              CsvReservationRepository &reservationRepository);

  bool borrowBook(int bookId, const std::string &memberId,
                  const std::string &borrowDate, const std::string &dueDate);

  bool returnBook(int loanId, const std::string &returnDate,
                  const std::string &nextDueDate);

  bool reserveBook(int bookId, const std::string &memberId,
                   const std::string &reservedAt);

  std::vector<Loan> listActiveLoans() const;
  std::vector<Loan> listLoanHistoryByMember(const std::string &memberId) const;
};