#pragma once

#include <xitren/comm/observer.hpp>

#include <xitren/unit/material.h>

#include <array>
#include <atomic>
#include <chrono>
#include <numeric>

namespace xitren::unit {

template <std::size_t Size>
class material_complex : public material {
    using components_type = std::array<material, Size>;
    using id_type         = std::string_view;

public:
    const id_type id;

    material_complex(id_type const& name, material::price_type const& price, components_type const& components)
        : id{name}, material{price}, components_{components}, comp_cost_{accumulate_costs(price, components_)}
    {}

    price_type
    cost()
    {
        return material::price() + comp_cost_;
    }

private:
    const components_type      components_{};
    const material::price_type comp_cost_{};

    material::price_type
    accumulate_costs(material::price_type const& price, components_type const& components) const
    {
        auto dash_fold = [](material::price_type a, material b) { return a + b.cost(); };
        return std::accumulate(components.begin(), components.end(), 0, dash_fold);
    }
};

}    // namespace xitren::unit
