cmake_minimum_required(VERSION 3.16)

project(OOPBorrowingSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(oop_project
    src/main.cpp
    src/models/Book.cpp
    src/models/User.cpp
    src/models/Admin.cpp
    src/models/Member.cpp
    src/models/Loan.cpp
    src/repositories/CsvBookRepository.cpp
)

target_include_directories(oop_project PRIVATE include)

if(MSVC)
    target_compile_options(oop_project PRIVATE /W4)
else()
    target_compile_options(oop_project PRIVATE -Wall -Wextra -pedantic)
endif()