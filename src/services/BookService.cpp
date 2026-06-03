#include "services/BookService.hpp"

BookService::BookService(CsvBookRepository &bookRepository,
                         CsvLoanRepository &loanRepository)
    : bookRepository_(bookRepository), loanRepository_(loanRepository) {}

bool BookService::addBook(const Book &book) {
  return bookRepository_.save(book);
}

bool BookService::updateBook(const Book &book) {
  auto existingBook = bookRepository_.findById(book.id());

  if (!existingBook.has_value()) {
    return false;
  }

  return bookRepository_.save(book);
}

bool BookService::deleteBook(int bookId) {
  for (const Loan &loan : loanRepository_.listActive()) {
    if (loan.bookId() == bookId) {
      return false;
    }
  }

  return bookRepository_.remove(bookId);
}

std::optional<Book> BookService::findBookById(int bookId) const {
  return bookRepository_.findById(bookId);
}

std::vector<Book> BookService::listBooks() const {
  return bookRepository_.listAll();
}

std::vector<Book> BookService::searchBooks(const std::string &keyword) const {
  return bookRepository_.search(keyword);
}