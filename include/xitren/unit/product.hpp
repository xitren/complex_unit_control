#pragma once

#include <xitren/unit/material.h>

#include <array>
#include <atomic>
#include <chrono>
#include <memory>
#include <numeric>

namespace xitren::unit {

template <material_types Type>
class product : public material {
public:
    product(std::size_t price)
        requires(get_material_class(Type) != material_class::solid)
        : material{Type, price, get_material_capacity(Type), get_material_class(Type)}
    {}
    product(std::size_t price, std::size_t capacity)
        requires(get_material_class(Type) == material_class::solid)
        : material{Type, price, capacity, get_material_class(Type)}
    {}
};

}    // namespace xitren::unit
