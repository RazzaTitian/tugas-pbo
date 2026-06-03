#include "models/Loan.hpp"

#include <utility>

Loan::Loan()
    : loanId_(0), bookId_(0), memberId_(""), borrowDate_(""), dueDate_(""),
      returnDate_(""), status_(LoanStatus::Active) {}

Loan::Loan(int loanId, int bookId, std::string memberId, std::string borrowDate,
           std::string dueDate, std::string returnDate, LoanStatus status)
    : loanId_(loanId), bookId_(bookId), memberId_(std::move(memberId)),
      borrowDate_(std::move(borrowDate)), dueDate_(std::move(dueDate)),
      returnDate_(std::move(returnDate)), status_(status) {}

int Loan::loanId() const { return loanId_; }

int Loan::bookId() const { return bookId_; }

const std::string &Loan::memberId() const { return memberId_; }

const std::string &Loan::borrowDate() const { return borrowDate_; }

const std::string &Loan::dueDate() const { return dueDate_; }

const std::string &Loan::returnDate() const { return returnDate_; }

LoanStatus Loan::status() const { return status_; }

void Loan::setReturnDate(const std::string &returnDate) {
  returnDate_ = returnDate;
}

void Loan::setStatus(LoanStatus status) { status_ = status; }

bool Loan::isActive() const { return status_ == LoanStatus::Active; }

bool Loan::isReturned() const { return status_ == LoanStatus::Returned; }

bool Loan::isOverdue(const std::string &today) const {
  return isActive() && !dueDate_.empty() && dueDate_ < today;
}

std::string Loan::statusText() const { return statusToString(status_); }

LoanStatus Loan::statusFromString(const std::string &status) {
  if (status == "RETURNED") {
    return LoanStatus::Returned;
  }

  return LoanStatus::Active;
}

std::string Loan::statusToString(LoanStatus status) {
  if (status == LoanStatus::Returned) {
    return "RETURNED";
  }

  return "ACTIVE";
}

bool Loan::operator==(const Loan &other) const {
  return loanId_ == other.loanId_;
}

std::ostream &operator<<(std::ostream &os, const Loan &loan) {
  os << "Loan{id=" << loan.loanId_ << ", bookId=" << loan.bookId_
     << ", memberId=\"" << loan.memberId_ << "\", borrowDate=\""
     << loan.borrowDate_ << "\", dueDate=\"" << loan.dueDate_
     << "\", returnDate=\"" << loan.returnDate_
     << "\", status=" << loan.statusText() << "}";

  return os;
}