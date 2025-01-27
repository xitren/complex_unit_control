#pragma once

#include <xitren/comm/observer.hpp>

#include <xitren/unit/material_types.h>

#include <array>
#include <atomic>
#include <chrono>

namespace xitren::unit {

class material : public xitren::comm::observer<std::size_t> {
    static constexpr std::size_t base_price{100UL};

public:
    using name_type                 = material_types;
    using id_type                   = std::size_t;
    using price_type                = std::size_t;
    using time_type                 = std::size_t;
    using capacity_type             = std::size_t;
    using atomic_id_type            = std::atomic<id_type>;
    using price_spent_follower_type = xitren::comm::observer<price_type>;

    name_type
    name() const;
    id_type
    id() const;
    time_type
    age() const;
    price_type
    price() const;
    price_type
    cost() const;
    capacity_type
    capacity() const;
    material_class
    type() const;

    material() = default;
    material(name_type const& name, price_type price, capacity_type capacity, material_class type)
        : name_{name}, price_{price}, capacity_{capacity}, type_{type}
    {}

protected:
    void
    data(void const* src, price_type const& nd) override;

    // void
    // capacity(capacity_type val);

private:
    static inline atomic_id_type id_pool{};
    const id_type                id_{id_pool++};
    const name_type              name_{name_type::helium};
    const time_type              birth_date_{now_in_ms()};
    const price_type             price_{base_price};
    const capacity_type          capacity_{1};
    const material_class         type_{material_class::solid};
    price_type                   cost_{};

    time_type
    now_in_ms() const;
};

}    // namespace xitren::unit
