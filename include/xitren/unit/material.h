#pragma once

#include <xitren/comm/observer.hpp>

#include <array>
#include <atomic>
#include <chrono>

namespace xitren::unit {

enum class material_class { gas, liquid, solid, container };

class material : public xitren::comm::observer<std::size_t> {
    static constexpr std::size_t base_price{100UL};

public:
    using name_type                 = std::string_view;
    using id_type                   = std::size_t;
    using price_type                = std::size_t;
    using time_type                 = std::size_t;
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

    material() = default;

protected:
    explicit material(name_type const& name, price_type price) : name_{name}, price_{price} {}

    void
    data(void const* src, price_type const& nd) override;

private:
    static inline atomic_id_type id_pool{};
    id_type                      id_{id_pool++};
    name_type                    name_{"Base"};
    time_type                    birth_date_{now_in_ms()};
    price_type                   price_{base_price};

    time_type
    now_in_ms() const;
};

}    // namespace xitren::unit
