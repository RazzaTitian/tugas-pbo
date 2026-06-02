#include "web/WebServer.hpp"

#include "external/httplib.h"

#include <iostream>

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

    server.Get("/", [](const httplib::Request&, httplib::Response& response) {
        response.set_content(
            "<h1>UGM Library Web Server</h1>"
            "<p>Web layer is running.</p>",
            "text/html"
        );
    });

    std::cout << "Web server running at http://localhost:" << port << '\n';

    server.listen("0.0.0.0", port);
}