#pragma once

#include <optional>
#include <vector>

template<typename T, typename Id>
class Repository {
public:
    virtual ~Repository() = default;

    virtual bool save(const T& object) = 0;

    virtual std::optional<T> findById(const Id& id) const = 0;

    virtual std::vector<T> listAll() const = 0;

    virtual bool remove(const Id& id) = 0;
};