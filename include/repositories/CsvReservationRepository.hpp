#pragma once

#include "models/Reservation.hpp"
#include "repositories/Repository.hpp"

#include <optional>
#include <string>
#include <vector>

class CsvReservationRepository : public Repository<Reservation, int> {
private:
  std::string filePath_;

  static Reservation fromCsvLine(const std::string &line);
  static std::string toCsvLine(const Reservation &reservation);

public:
  explicit CsvReservationRepository(std::string filePath);

  bool save(const Reservation &reservation) override;
  std::optional<Reservation> findById(const int &id) const override;
  std::vector<Reservation> listAll() const override;
  bool remove(const int &id) override;

  std::vector<Reservation> listWaitingByBookId(int bookId) const;
  std::vector<Reservation> listByMemberId(const std::string &memberId) const;

  std::optional<Reservation> findFirstWaitingByBookId(int bookId) const;

  int nextId() const;
};