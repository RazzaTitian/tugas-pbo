# OOP Borrowing System

Mini Project for Object-Oriented Programming (C++)
Department of Electrical Engineering and Information Technology (DTETI)
Universitas Gadjah Mada

## Author

- Name: Muhammad Razza Titian Jiwani
- NIM: [INSERT NIM HERE]
- Course: Object-Oriented Programming (C++)

---

## Project Description

OOP Borrowing System is a borrowing management application developed using C++17 and Object-Oriented Programming principles.

The system allows administrators to manage books, members, loans, reservations, and borrowing workflows through a command-line interface while maintaining persistent data storage using CSV files.

The architecture follows a layered design:

```txt
CLI Layer
    ↓
Service Layer
    ↓
Repository Layer
    ↓
CSV Persistence Layer
```

---

## Features

### Core Features

- Book management
- Member management
- Admin management
- Loan management
- Borrow book workflow
- Return book workflow
- Persistent CSV storage

### Additional Features

- Generic Repository<T, ID>
- Book search
- Loan history
- Password hashing
- Reservation queue
- Service layer architecture
- Admin authentication

---

## OOP Concepts Used

### Encapsulation

All class attributes are private or protected and accessed through public methods.

### Inheritance

```txt
User
├── Admin
└── Member
```

### Polymorphism

The `User` class defines a pure virtual method:

```cpp
virtual std::string role() const = 0;
```

which is implemented by derived classes.

### Operator Overloading

Implemented using:

```cpp
operator==
operator<<
```

for model classes.

### Generic Programming

Implemented using:

```cpp
template<typename T, typename ID>
class Repository
```

---

## UML Class Diagram

### Diagram

![Week 1 UML Diagram](docs/week1_class_diagram/uml.png)

### PlantUML Source

- [week1_class_diagram.puml](docs/week1_class_diagram.puml)

---

## Project Structure

```txt
include/
├── cli/
├── models/
├── repositories/
├── services/
└── utils/

src/
├── cli/
├── models/
├── repositories/
├── services/
└── utils/

data/
├── books.csv
├── members.csv
├── loans.csv
├── reservations.csv
└── admins.csv

docs/
└── week1_class_diagram.puml
```

---

## Persistence Layer

CSV repositories:

- CsvBookRepository
- CsvMemberRepository
- CsvAdminRepository
- CsvLoanRepository
- CsvReservationRepository

Supported operations:

```cpp
save()
findById()
listAll()
remove()
```

Data persists across multiple program executions.

---

## Week 1 Deliverables

### Core OOP Engine

Implemented classes:

- User (abstract)
- Admin
- Member
- Book
- Loan
- Reservation

### Database Layer

Implemented CSV persistence using:

- CsvBookRepository
- CsvMemberRepository
- CsvAdminRepository
- CsvLoanRepository
- CsvReservationRepository

### Sample Data

The project includes sample data stored in:

- books.csv
- members.csv
- loans.csv
- reservations.csv
- admins.csv

Data persists across multiple program executions.

---

## Build Instructions

### Requirements

- C++17
- CMake 3.16+
- Visual Studio Build Tools / MSVC

### Build

```bash
cmake -B build
cmake --build build --config Debug
```

### Run

```bash
.\build\Debug\oop_project.exe
```

---

## Current Progress

### Week 1

Completed:

- UML design
- Core OOP models
- Repository layer
- CSV persistence
- Borrowing workflow
- Return workflow

### Week 2

In Progress:

- Admin CLI
- CRUD menus
- Validation
- Reporting

### Week 3

Planned:

- Web interface
- HTTP routes
- User borrowing portal

---

## License

Educational project developed for DTETI UGM.
