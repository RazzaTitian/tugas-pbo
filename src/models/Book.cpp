#include "../../include/models/Book.hpp"
#include <utility>

Book::Book() : id_(0), title_(""), author_(""), available_(true) {}

Book::Book(int id, std::string title, std::string author, bool available)
    : id_(id), title_(std::move(title)), author_(std::move(author)),
      available_(available) {}

int Book::id() const { return id_; }

const std::string &Book::title() const { return title_; }

const std::string &Book::author() const { return author_; }

bool Book::isAvailable() const { return available_; }

void Book::setTitle(const std::string &title) { title_ = title; }

void Book::setAuthor(const std::string &author) { author_ = author; }

void Book::setAvailable(bool available) { available_ = available; }

std::string Book::statusText() const {
  return available_ ? "available" : "on loan";
}

bool Book::operator==(const Book &other) const { return id_ == other.id_; }

std::ostream &operator<<(std::ostream &os, const Book &book) {
  os << "Book{id=" << book.id_ << ", title=\"" << book.title_ << "\", author=\""
     << book.author_ << "\", status=" << book.statusText() << "}";
  return os;
}