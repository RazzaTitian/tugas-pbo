#include "models/Reservation.hpp"

#include <utility>

Reservation::Reservation()
    : reservationId_(0),
      bookId_(0),
      memberId_(""),
      reservedAt_(""),
      status_(ReservationStatus::Waiting) {}

Reservation::Reservation(
    int reservationId,
    int bookId,
    std::string memberId,
    std::string reservedAt,
    ReservationStatus status
)
    : reservationId_(reservationId),
      bookId_(bookId),
      memberId_(std::move(memberId)),
      reservedAt_(std::move(reservedAt)),
      status_(status) {}

int Reservation::reservationId() const {
    return reservationId_;
}

int Reservation::bookId() const {
    return bookId_;
}

const std::string& Reservation::memberId() const {
    return memberId_;
}

const std::string& Reservation::reservedAt() const {
    return reservedAt_;
}

ReservationStatus Reservation::status() const {
    return status_;
}

void Reservation::setStatus(ReservationStatus status) {
    status_ = status;
}

bool Reservation::isWaiting() const {
    return status_ == ReservationStatus::Waiting;
}

bool Reservation::isFulfilled() const {
    return status_ == ReservationStatus::Fulfilled;
}

bool Reservation::isCancelled() const {
    return status_ == ReservationStatus::Cancelled;
}

std::string Reservation::statusText() const {
    return statusToString(status_);
}

ReservationStatus Reservation::statusFromString(const std::string& status) {
    if (status == "FULFILLED") {
        return ReservationStatus::Fulfilled;
    }

    if (status == "CANCELLED") {
        return ReservationStatus::Cancelled;
    }

    return ReservationStatus::Waiting;
}

std::string Reservation::statusToString(ReservationStatus status) {
    if (status == ReservationStatus::Fulfilled) {
        return "FULFILLED";
    }

    if (status == ReservationStatus::Cancelled) {
        return "CANCELLED";
    }

    return "WAITING";
}

bool Reservation::operator==(const Reservation& other) const {
    return reservationId_ == other.reservationId_;
}

std::ostream& operator<<(std::ostream& os, const Reservation& reservation) {
    os << "Reservation{id=" << reservation.reservationId_
       << ", bookId=" << reservation.bookId_
       << ", memberId=\"" << reservation.memberId_
       << "\", reservedAt=\"" << reservation.reservedAt_
       << "\", status=" << reservation.statusText()
       << "}";

    return os;
}