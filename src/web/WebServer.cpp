#include "web/WebServer.hpp"

#include "external/httplib.h"
#include "models/Book.hpp"

#include <iostream>
#include <string>
#include <vector>

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
        html += "<html>";
        html += "<head>";
        html += "<meta charset='UTF-8'>";
        html += "<title>UGM Library</title>";
        html += "</head>";
        html += "<body>";

        html += "<h1>UGM Library</h1>";

        html += "<form action='/search' method='get'>";
        html += "<label>Search: </label>";
        html += "<input type='text' name='q'>";
        html += "<button type='submit'>Search</button>";
        html += "</form>";

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

                html += "<div style='margin-bottom: 12px;'>";
                html += "<strong>";
                html += book.title();
                html += "</strong>";
                html += " - ";
                html += book.author();
                html += " ";
                html += "<span style='color: green;'>(Available)</span>";
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

    std::cout << "Web server running at http://localhost:" << port << '\n';

    server.listen("0.0.0.0", port);
}