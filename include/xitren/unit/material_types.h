#pragma once

#include <array>
#include <atomic>
#include <chrono>
#include <memory>
#include <numeric>

namespace xitren::unit {

enum class material_class { gas, liquid, solid, container };

enum class material_types {
    //===== Resources ==============
    // Solid
    ice,
    ore_iron,
    ore_copper,
    ore_carbon,
    ore_sillicon,
    ore_nickel,
    ore_gold,
    ore_titanium,
    ore_uranium,

    // Gas
    hydrogen,
    helium,
    nitrogen,
    carbon_dioxide,
    carbon_monoxide,
    methane,
    ethane,
    propane,

    // Liquid
    water,
    coolant,
    oil,
    fuel,
    nanites,

    // Containers

    //===== Materials ==============
    iron,
    copper,
    carbon,
    sillicon,
    nickel,
    gold,
    titanium,
    uranium,
    //===== Technological ==============
    hull_parts,
    engines,
    microchips,
    sense_arrays,
    quantum_tubes,
    plasma_conductors,
    composits,
    //===== Component ==============
    drone_components,
    electronics,
    hi_tech,
    turret_components,
    field_coils,
    shield_components,
    weapon_components,
    missile_components,
    antimatter_converters,
    smart_chips,
    //===== Product ==============
    station_modules,
    ship_chasis,
    shields,
    engine_trusters,
    turrets,
    satellites,
    weapons,
    mines,
    missiles,
    drones
};

constexpr material_class
get_material_class(const material_types type)
{
    if (type == material_types::ice || type == material_types::ore_iron || type == material_types::ore_copper
        || type == material_types::ore_carbon || type == material_types::ore_sillicon
        || type == material_types::ore_nickel || type == material_types::ore_gold
        || type == material_types::ore_titanium || type == material_types::ore_uranium) {
        return material_class::solid;
    } else if (type == material_types::hydrogen || type == material_types::helium || type == material_types::nitrogen
               || type == material_types::carbon_dioxide || type == material_types::carbon_monoxide
               || type == material_types::methane || type == material_types::ethane
               || type == material_types::propane) {
        return material_class::gas;
    } else if (type == material_types::water || type == material_types::coolant || type == material_types::fuel
               || type == material_types::nanites || type == material_types::oil) {
        return material_class::liquid;
    }
    return material_class::container;
};

constexpr std::size_t
get_material_capacity(const material_types type)
{
    if (get_material_class(type) == material_class::solid) {
        return 0;
    }
    if (get_material_class(type) == material_class::gas || get_material_class(type) == material_class::liquid) {
        return 1;
    }
    switch (type) {
    case material_types::microchips:
    case material_types::smart_chips:
        return 1;
    case material_types::electronics:
    case material_types::hi_tech:
        return 50;
    case material_types::iron:
    case material_types::copper:
    case material_types::carbon:
    case material_types::sillicon:
    case material_types::nickel:
    case material_types::gold:
    case material_types::titanium:
    case material_types::uranium:
    case material_types::quantum_tubes:
    case material_types::plasma_conductors:
    case material_types::composits:
        return 100;
    case material_types::drone_components:
    case material_types::turret_components:
    case material_types::field_coils:
    case material_types::shield_components:
    case material_types::weapon_components:
    case material_types::missile_components:
    case material_types::antimatter_converters:
        return 400;
    case material_types::sense_arrays:
    case material_types::hull_parts:
    case material_types::engines:
        return 1000;
    case material_types::shields:
    case material_types::turrets:
    case material_types::satellites:
    case material_types::weapons:
    case material_types::mines:
    case material_types::missiles:
    case material_types::drones:
        return 10'000;
    case material_types::station_modules:
    case material_types::ship_chasis:
    case material_types::engine_trusters:
        return 500'000;
    default:
        return 1;
    }
};

}    // namespace xitren::unit
