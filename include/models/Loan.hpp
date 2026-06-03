#pragma once

#include <iostream>
#include <string>

enum class LoanStatus { Active, Returned };

class Loan {
private:
  int loanId_;
  int bookId_;
  std::string memberId_;
  std::string borrowDate_;
  std::string dueDate_;
  std::string returnDate_;
  LoanStatus status_;

public:
  Loan();

  Loan(int loanId, int bookId, std::string memberId, std::string borrowDate,
       std::string dueDate, std::string returnDate, LoanStatus status);

  int loanId() const;
  int bookId() const;
  const std::string &memberId() const;
  const std::string &borrowDate() const;
  const std::string &dueDate() const;
  const std::string &returnDate() const;
  LoanStatus status() const;

  void setReturnDate(const std::string &returnDate);
  void setStatus(LoanStatus status);

  bool isActive() const;
  bool isReturned() const;
  bool isOverdue(const std::string &today) const;

  std::string statusText() const;

  static LoanStatus statusFromString(const std::string &status);
  static std::string statusToString(LoanStatus status);

  bool operator==(const Loan &other) const;

  friend std::ostream &operator<<(std::ostream &os, const Loan &loan);
};