#pragma once

#include <iostream>
#include <string>

enum class ReservationStatus {
    Waiting,
    Fulfilled,
    Cancelled
};

class Reservation {
private:
    int reservationId_;
    int bookId_;
    std::string memberId_;
    std::string reservedAt_;
    ReservationStatus status_;

public:
    Reservation();

    Reservation(
        int reservationId,
        int bookId,
        std::string memberId,
        std::string reservedAt,
        ReservationStatus status
    );

    int reservationId() const;
    int bookId() const;
    const std::string& memberId() const;
    const std::string& reservedAt() const;
    ReservationStatus status() const;

    void setStatus(ReservationStatus status);

    bool isWaiting() const;
    bool isFulfilled() const;
    bool isCancelled() const;

    std::string statusText() const;

    static ReservationStatus statusFromString(const std::string& status);
    static std::string statusToString(ReservationStatus status);

    bool operator==(const Reservation& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Reservation& reservation);
};