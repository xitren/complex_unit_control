#pragma once

#include <xitren/comm/values/observable.hpp>

#include <xitren/storage/type.h>
#include <xitren/unit/material.h>

#include <array>
#include <bitset>
#include <memory>

namespace xitren::storage {

enum class storage_status_type {
    ok,
    loadded_partually,
    storage_is_full,
    no_place_for_new_type,
    doesnt_exist,
    bad_material_type
};

template <xitren::storage::capacity Cap, template <xitren::storage::capacity> class Derived>
class istore : public comm::values::observed<std::size_t> {
public:
    using mat_type   = std::unique_ptr<unit::material>;
    using value_type = std::size_t;

    istore() : comm::values::observed<value_type>{[&](value_type val) { cost_ += val; }} {}

    int
    capacity() const
    {
        return static_cast<int>(Cap);
    }

    int
    load() const
    {
        return load_;
    }

    storage_status_type
    push(std::unique_ptr<unit::material> mat)
    {
        return static_cast<Derived<Cap> const*>(this)->push(std::move(mat));
    }

    mat_type
    pull(unit::material::name_type const& mat_id)
    {
        return static_cast<Derived<Cap> const*>(this)->pull(mat_id);
    }

    void
    reset_cost(value_type val)
    {
        cost_ = 0;
    }

protected:
    value_type load_{};
    value_type cost_{};

    value_type
    mean_price(value_type price1, value_type cap1, value_type price2, value_type cap2)
    {
        return (price1 * cap1 + price2 * cap2) / (cap1 + cap2);
    }
};

}    // namespace xitren::storage
