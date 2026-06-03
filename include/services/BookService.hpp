#pragma once

#include "models/Book.hpp"
#include "repositories/CsvBookRepository.hpp"
#include "repositories/CsvLoanRepository.hpp"

#include <optional>
#include <string>
#include <vector>

class BookService {
private:
  CsvBookRepository &bookRepository_;
  CsvLoanRepository &loanRepository_;

public:
  BookService(CsvBookRepository &bookRepository,
              CsvLoanRepository &loanRepository);

  bool addBook(const Book &book);
  bool updateBook(const Book &book);
  bool deleteBook(int bookId);

  std::optional<Book> findBookById(int bookId) const;
  std::vector<Book> listBooks() const;
  std::vector<Book> searchBooks(const std::string &keyword) const;
};