#include "web/WebServer.hpp"

#include "external/httplib.h"
#include "models/Book.hpp"
#include "models/Member.hpp"
#include "models/Loan.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace {
std::string escapeJson(const std::string& text) {
    std::string escaped;

    for (char character : text) {
        if (character == '"') {
            escaped += "\\\"";
        } else if (character == '\\') {
            escaped += "\\\\";
        } else if (character == '\n') {
            escaped += "\\n";
        } else if (character == '\r') {
            escaped += "\\r";
        } else if (character == '\t') {
            escaped += "\\t";
        } else {
            escaped += character;
        }
    }

    return escaped;
}
}

WebServer::WebServer(
    BookService& bookService,
    MemberService& memberService,
    LoanService& loanService
)
    : bookService_(bookService),
      memberService_(memberService),
      loanService_(loanService) {}

void WebServer::run(int port) {
    httplib::Server server;

    server.Get("/", [this](const httplib::Request&, httplib::Response& response) {
        std::string html;

        html += "<!DOCTYPE html>";
        html += "<head>";
        html += "<meta charset='UTF-8'>";
        html += "<title>UGM Library</title>";
        html += "<style>";
        html += "body { font-family: Arial, sans-serif; max-width: 900px; margin: 40px auto; padding: 0 20px; }";
        html += "h1 { margin-bottom: 4px; }";
        html += ".subtitle { color: #555; margin-top: 0; }";
        html += ".nav-box { display: flex; gap: 16px; flex-wrap: wrap; margin: 24px 0; }";
        html += ".card { border: 1px solid #ddd; border-radius: 8px; padding: 16px; margin-bottom: 16px; }";
        html += ".book-card { border: 1px solid #ddd; border-radius: 8px; padding: 16px; margin-bottom: 14px; }";
        html += ".available { color: green; font-weight: bold; }";
        html += "input { padding: 6px; margin: 4px 0; }";
        html += "button { padding: 6px 10px; cursor: pointer; }";
        html += "form { margin-top: 8px; }";
        html += "</style>";
        html += "</head>";
        html += "<body>";

        html += "<h1>UGM Library</h1>";
        html += "<p class='subtitle'>Borrowing system powered by C++17, CSV, and cpp-httplib.</p>";

        html += "<div class='nav-box'>";

        html += "<div class='card'>";
        html += "<h3>Search Books</h3>";
        html += "<form action='/search' method='get'>";
        html += "<input type='text' name='q' placeholder='Title or author'>";
        html += "<br>";
        html += "<button type='submit'>Search</button>";
        html += "</form>";
        html += "</div>";

        html += "<div class='card'>";
        html += "<h3>My Loans</h3>";
        html += "<form action='/me' method='get'>";
        html += "<input type='text' name='id' placeholder='Member ID'>";
        html += "<br>";
        html += "<button type='submit'>View My Loans</button>";
        html += "</form>";
        html += "</div>";

        html += "</div>";

        html += "<h2>Available Books</h2>";

        std::vector<Book> books = bookService_.listBooks();

        if (books.empty()) {
        html += "<p>No books found.</p>";
    } else {
        bool hasAvailableBook = false;

        for (const Book& book : books) {
            if (!book.isAvailable()) {
                continue;
            }

            hasAvailableBook = true;

            html += "<div class='book-card'>";

            html += "<h3>";
            html += book.title();
            html += "</h3>";

            html += "<p>Author: ";
            html += book.author();
            html += "</p>";

            html += "<p class='available'>Available</p>";

            html += "<form action='/borrow' method='post'>";
            html += "<input type='hidden' name='bookId' value='";
            html += std::to_string(book.id());
            html += "'>";
            html += "<input type='text' name='memberId' placeholder='Member ID'>";
            html += "<br>";
            html += "<button type='submit'>Borrow</button>";
            html += "</form>";

            html += "</div>";
        }

        if (!hasAvailableBook) {
            html += "<p>No available books at the moment.</p>";
        }
    }

    html += "</body>";
    html += "</html>";

    response.set_content(html, "text/html");
});

    server.Get("/search", [this](const httplib::Request& request,
                                 httplib::Response& response) {
        std::string keyword;

        if (request.has_param("q")) {
            keyword = request.get_param_value("q");
        }

        std::string html;

        html += "<!DOCTYPE html>";
        html += "<html>";
        html += "<head>";
        html += "<meta charset='UTF-8'>";
        html += "<title>Search Results</title>";
        html += "</head>";
        html += "<body>";

        html += "<h1>Search Results</h1>";
        html += "<a href='/'>Back to Home</a><br><br>";

        if (keyword.empty()) {
            html += "<p>No search keyword provided.</p>";
            html += "</body>";
            html += "</html>";

            response.set_content(html, "text/html");
            return;
        }

        html += "<p>Keyword: <strong>";
        html += keyword;
        html += "</strong></p>";

        std::vector<Book> books = bookService_.searchBooks(keyword);

        if (books.empty()) {
            html += "<p>No matching books found.</p>";
        } else {
            for (const Book& book : books) {
                html += "<div style='margin-bottom:12px;'>";

                html += "<strong>";
                html += book.title();
                html += "</strong>";

                html += " - ";
                html += book.author();

                if (book.isAvailable()) {
                    html += " <span style='color:green;'>(Available)</span>";
                } else {
                    html += " <span style='color:red;'>(On Loan)</span>";
                }

                html += "</div>";
            }
        }

        html += "</body>";
        html += "</html>";

        response.set_content(html, "text/html");
    });

    server.Get("/me", [this](const httplib::Request& request,
                         httplib::Response& response) {
    std::string memberId;

    if (request.has_param("id")) {
        memberId = request.get_param_value("id");
    }

    std::string html;

    html += "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += "<meta charset='UTF-8'>";
    html += "<title>My Loans</title>";
    html += "</head>";
    html += "<body>";

    html += "<h1>My Loans</h1>";
    html += "<a href='/'>Back to Home</a><br><br>";

    if (memberId.empty()) {
        html += "<p>No member ID provided.</p>";
        html += "</body>";
        html += "</html>";

        response.set_content(html, "text/html");
        return;
    }

    auto memberOpt = memberService_.findMemberById(memberId);

    if (!memberOpt.has_value()) {
        html += "<p>Member not found.</p>";
        html += "</body>";
        html += "</html>";

        response.set_content(html, "text/html");
        return;
    }

    Member member = memberOpt.value();

    html += "<p>Hello, <strong>";
    html += member.name();
    html += "</strong></p>";

    std::vector<Loan> loans = memberService_.listMemberLoans(memberId);

    if (loans.empty()) {
        html += "<p>No loan history found.</p>";
    } else {
        html += "<h2>Loan History</h2>";

        for (const Loan& loan : loans) {
            html += "<div style='margin-bottom:12px;'>";

            html += "Loan ID: <strong>";
            html += std::to_string(loan.loanId());
            html += "</strong><br>";

            html += "Book ID: ";
            html += std::to_string(loan.bookId());
            html += "<br>";

            html += "Borrow date: ";
            html += loan.borrowDate();
            html += "<br>";

            html += "Due date: ";
            html += loan.dueDate();
            html += "<br>";

            html += "Return date: ";
            html += loan.returnDate().empty()
                        ? "-"
                        : loan.returnDate();
            html += "<br>";

            html += "Status: ";
            html += loan.statusText();

            if (loan.isActive()) {
                html += "<form action='/return' method='post' style='margin-top: 4px;'>";
                html += "<input type='hidden' name='loanId' value='";
                html += std::to_string(loan.loanId());
                html += "'>";
                html += "<button type='submit'>Return</button>";
                html += "</form>";
            }

            html += "</div>";
        }
    }

    html += "</body>";
    html += "</html>";

    response.set_content(html, "text/html");
});

    server.Post("/borrow", [this](const httplib::Request& request,
                              httplib::Response& response) {
    std::string bookIdValue;
    std::string memberId;

    if (request.has_param("bookId")) {
        bookIdValue = request.get_param_value("bookId");
    }

    if (request.has_param("memberId")) {
        memberId = request.get_param_value("memberId");
    }

    std::string html;

    html += "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += "<meta charset='UTF-8'>";
    html += "<title>Borrow Book</title>";
    html += "</head>";
    html += "<body>";

    html += "<h1>Borrow Book</h1>";
    html += "<a href='/'>Back to Home</a><br><br>";

    if (bookIdValue.empty() || memberId.empty()) {
        html += "<p>Missing book ID or member ID.</p>";
        html += "</body>";
        html += "</html>";

        response.set_content(html, "text/html");
        return;
    }

    int bookId = 0;

    try {
        bookId = std::stoi(bookIdValue);
    } catch (const std::invalid_argument&) {
        html += "<p>Invalid book ID.</p>";
        html += "</body>";
        html += "</html>";

        response.set_content(html, "text/html");
        return;
    } catch (const std::out_of_range&) {
        html += "<p>Invalid book ID.</p>";
        html += "</body>";
        html += "</html>";

        response.set_content(html, "text/html");
        return;
    }

    bool borrowOk = loanService_.borrowBook(
        bookId,
        memberId,
        "2026-06-03",
        "2026-06-17"
    );

    if (borrowOk) {
        html += "<p style='color: green;'>Book borrowed successfully.</p>";
        html += "<p>Member ID: ";
        html += memberId;
        html += "</p>";
    } else {
        html += "<p style='color: red;'>Failed to borrow book.</p>";
        html += "<p>The book may be unavailable, or the member ID may be invalid.</p>";
    }

    html += "</body>";
    html += "</html>";

    response.set_content(html, "text/html");
});

    server.Post("/return", [this](const httplib::Request& request,
                              httplib::Response& response) {
    std::string loanIdValue;

    if (request.has_param("loanId")) {
        loanIdValue = request.get_param_value("loanId");
    }

    std::string html;

    html += "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += "<meta charset='UTF-8'>";
    html += "<title>Return Book</title>";
    html += "</head>";
    html += "<body>";

    html += "<h1>Return Book</h1>";
    html += "<a href='/'>Back to Home</a><br><br>";

    if (loanIdValue.empty()) {
        html += "<p>Missing loan ID.</p>";
        html += "</body>";
        html += "</html>";

        response.set_content(html, "text/html");
        return;
    }

    int loanId = 0;

    try {
        loanId = std::stoi(loanIdValue);
    } catch (const std::invalid_argument&) {
        html += "<p>Invalid loan ID.</p>";
        html += "</body>";
        html += "</html>";

        response.set_content(html, "text/html");
        return;
    } catch (const std::out_of_range&) {
        html += "<p>Invalid loan ID.</p>";
        html += "</body>";
        html += "</html>";

        response.set_content(html, "text/html");
        return;
    }

    bool returnOk = loanService_.returnBook(
        loanId,
        "2026-06-03",
        "2026-06-17"
    );

    if (returnOk) {
        html += "<p style='color: green;'>Book returned successfully.</p>";
        html += "<p>If there was a reservation queue, the book was assigned automatically.</p>";
    } else {
        html += "<p style='color: red;'>Failed to return book.</p>";
        html += "<p>The loan ID may be invalid or already returned.</p>";
    }

    html += "</body>";
    html += "</html>";

    response.set_content(html, "text/html");
});

    server.Get("/api/books", [this](const httplib::Request&,
                                httplib::Response& response) {
    std::vector<Book> books = bookService_.listBooks();

    std::string json;

    json += "[";

    for (std::size_t index = 0; index < books.size(); ++index) {
        const Book& book = books[index];

        json += "{";

        json += "\"id\":";
        json += std::to_string(book.id());
        json += ",";

        json += "\"title\":\"";
        json += escapeJson(book.title());
        json += "\",";

        json += "\"author\":\"";
        json += escapeJson(book.author());
        json += "\",";

        json += "\"available\":";
        json += book.isAvailable() ? "true" : "false";

        json += "}";

        if (index + 1 < books.size()) {
            json += ",";
        }
    }

    json += "]";

    response.set_content(json, "application/json");
});

    server.Get("/api/members", [this](const httplib::Request&,
                                  httplib::Response& response) {
    std::vector<Member> members = memberService_.listMembers();

    std::string json;

    json += "[";

    for (std::size_t index = 0; index < members.size(); ++index) {
        const Member& member = members[index];

        json += "{";

        json += "\"memberId\":\"";
        json += escapeJson(member.memberId());
        json += "\",";

        json += "\"username\":\"";
        json += escapeJson(member.username());
        json += "\",";

        json += "\"name\":\"";
        json += escapeJson(member.name());
        json += "\",";

        json += "\"email\":\"";
        json += escapeJson(member.email());
        json += "\"";

        json += "}";

        if (index + 1 < members.size()) {
            json += ",";
        }
    }

    json += "]";

    response.set_content(json, "application/json");
});

    server.Get("/api/loans", [this](const httplib::Request&,
                                httplib::Response& response) {
    std::vector<Loan> loans = loanService_.listActiveLoans();

    std::string json;

    json += "[";

    for (std::size_t index = 0; index < loans.size(); ++index) {
        const Loan& loan = loans[index];

        json += "{";

        json += "\"loanId\":";
        json += std::to_string(loan.loanId());
        json += ",";

        json += "\"bookId\":";
        json += std::to_string(loan.bookId());
        json += ",";

        json += "\"memberId\":\"";
        json += escapeJson(loan.memberId());
        json += "\",";

        json += "\"borrowDate\":\"";
        json += escapeJson(loan.borrowDate());
        json += "\",";

        json += "\"dueDate\":\"";
        json += escapeJson(loan.dueDate());
        json += "\",";

        json += "\"returnDate\":\"";
        json += escapeJson(loan.returnDate());
        json += "\",";

        json += "\"status\":\"";
        json += escapeJson(loan.statusText());
        json += "\"";

        json += "}";

        if (index + 1 < loans.size()) {
            json += ",";
        }
    }

    json += "]";

    response.set_content(json, "application/json");
});

    std::cout << "Web server running at http://localhost:" << port << '\n';

    server.listen("0.0.0.0", port);
}