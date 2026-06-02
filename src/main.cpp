#include "repositories/CsvBookRepository.hpp"

#include <iostream>

int main() {
    CsvBookRepository repository("data/books.csv");

    repository.save(
        Book(
            1,
            "Clean Code",
            "Robert C. Martin",
            true
        )
    );

    repository.save(
        Book(
            2,
            "Effective Modern C++",
            "Scott Meyers",
            true
        )
    );

    std::cout << "Books currently stored:\n\n";

    for (const Book& book : repository.listAll()) {
        std::cout << book << '\n';
    }

    return 0;
}