#pragma once

#include <xitren/comm/observer.hpp>

#include <xitren/unit/material.h>

#include <array>
#include <atomic>
#include <chrono>
#include <memory>
#include <numeric>

namespace xitren::unit {

template <std::size_t Size>
class material_complex : public material {
    using components_type = std::array<material::price_type, Size>;

public:
    const name_type id;

    material_complex(name_type const& name, material::price_type const& price, components_type const& components,
                     material_class type)
        : id{name}, material{name, price, Size, type}, comp_cost_{accumulate_costs(price, components, Size)}
    {}

    price_type
    cost()
    {
        return material::price() + comp_cost_;
    }

private:
    const material::price_type comp_cost_{};

    material::price_type
    accumulate_costs(material::price_type const& price, components_type const& components) const
    {
        auto dash_fold = [](material::price_type a, material::price_type b) { return a + b; };
        return std::accumulate(components.begin(), components.end(), 0, dash_fold);
    }
};

}    // namespace xitren::unit
