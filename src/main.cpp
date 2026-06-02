#include "repositories/CsvBookRepository.hpp"
#include "repositories/CsvLoanRepository.hpp"
#include "services/BookService.hpp"

#include <iostream>

int main() {
    CsvBookRepository bookRepository("data/books.csv");
    CsvLoanRepository loanRepository("data/loans.csv");

    BookService bookService(bookRepository, loanRepository);

    bool deleteBook3 = bookService.deleteBook(3);
    bool deleteBook1 = bookService.deleteBook(1);

    std::cout << "Delete book 3: " << (deleteBook3 ? "success" : "failed") << '\n';
    std::cout << "Delete book 1: " << (deleteBook1 ? "success" : "failed") << '\n';

    std::cout << "\nBooks:\n";
    for (const Book& book : bookService.listBooks()) {
        std::cout << book << '\n';
    }

    return 0;
}