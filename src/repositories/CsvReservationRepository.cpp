#include "repositories/CsvReservationRepository.hpp"

#include <fstream>
#include <sstream>
#include <utility>
#include <vector>

CsvReservationRepository::CsvReservationRepository(std::string filePath)
    : filePath_(std::move(filePath)) {}

Reservation CsvReservationRepository::fromCsvLine(const std::string &line) {
  std::stringstream ss(line);

  std::string reservationIdStr;
  std::string bookIdStr;
  std::string memberId;
  std::string reservedAt;
  std::string status;

  std::getline(ss, reservationIdStr, ',');
  std::getline(ss, bookIdStr, ',');
  std::getline(ss, memberId, ',');
  std::getline(ss, reservedAt, ',');
  std::getline(ss, status, ',');

  return Reservation(std::stoi(reservationIdStr), std::stoi(bookIdStr),
                     memberId, reservedAt,
                     Reservation::statusFromString(status));
}

std::string
CsvReservationRepository::toCsvLine(const Reservation &reservation) {
  std::stringstream ss;

  ss << reservation.reservationId() << "," << reservation.bookId() << ","
     << reservation.memberId() << "," << reservation.reservedAt() << ","
     << reservation.statusText();

  return ss.str();
}

bool CsvReservationRepository::save(const Reservation &reservation) {
  std::vector<Reservation> reservations = listAll();
  bool updated = false;

  for (Reservation &existingReservation : reservations) {
    if (existingReservation.reservationId() == reservation.reservationId()) {
      existingReservation = reservation;
      updated = true;
      break;
    }
  }

  if (!updated) {
    reservations.push_back(reservation);
  }

  std::ofstream file(filePath_, std::ios::trunc);
  if (!file.is_open()) {
    return false;
  }

  for (const Reservation &currentReservation : reservations) {
    file << toCsvLine(currentReservation) << '\n';
  }

  return true;
}

std::optional<Reservation>
CsvReservationRepository::findById(const int &id) const {
  for (const Reservation &reservation : listAll()) {
    if (reservation.reservationId() == id) {
      return reservation;
    }
  }

  return std::nullopt;
}

std::vector<Reservation> CsvReservationRepository::listAll() const {
  std::vector<Reservation> reservations;
  std::ifstream file(filePath_);

  if (!file.is_open()) {
    return reservations;
  }

  std::string line;

  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    reservations.push_back(fromCsvLine(line));
  }

  return reservations;
}

bool CsvReservationRepository::remove(const int &id) {
  std::vector<Reservation> reservations = listAll();
  std::vector<Reservation> remainingReservations;

  bool removed = false;

  for (const Reservation &reservation : reservations) {
    if (reservation.reservationId() == id) {
      removed = true;
      continue;
    }

    remainingReservations.push_back(reservation);
  }

  if (!removed) {
    return false;
  }

  std::ofstream file(filePath_, std::ios::trunc);
  if (!file.is_open()) {
    return false;
  }

  for (const Reservation &reservation : remainingReservations) {
    file << toCsvLine(reservation) << '\n';
  }

  return true;
}

std::vector<Reservation>
CsvReservationRepository::listWaitingByBookId(int bookId) const {
  std::vector<Reservation> waitingReservations;

  for (const Reservation &reservation : listAll()) {
    if (reservation.bookId() == bookId && reservation.isWaiting()) {
      waitingReservations.push_back(reservation);
    }
  }

  return waitingReservations;
}

std::vector<Reservation>
CsvReservationRepository::listByMemberId(const std::string &memberId) const {
  std::vector<Reservation> memberReservations;

  for (const Reservation &reservation : listAll()) {
    if (reservation.memberId() == memberId) {
      memberReservations.push_back(reservation);
    }
  }

  return memberReservations;
}

std::optional<Reservation>
CsvReservationRepository::findFirstWaitingByBookId(int bookId) const {
  std::vector<Reservation> waitingReservations = listWaitingByBookId(bookId);

  if (waitingReservations.empty()) {
    return std::nullopt;
  }

  return waitingReservations.front();
}

int CsvReservationRepository::nextId() const {
  int highestId = 0;

  for (const Reservation &reservation : listAll()) {
    if (reservation.reservationId() > highestId) {
      highestId = reservation.reservationId();
    }
  }

  return highestId + 1;
}