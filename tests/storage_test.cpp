#include <xitren/storage/gas_liquid.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <iostream>
#include <string_view>

using namespace xitren::storage;
using namespace xitren::unit;
using namespace std::literals::string_view_literals;

TEST(storage_test, gas_liquid_storage)
{
    constexpr std::size_t base_cap{100UL};
    constexpr std::size_t base_price{100UL};

    auto mat1 = std::make_unique<material>("Base gas"sv, base_price, base_cap, material_class::gas);
    auto mat2 = std::make_unique<material>("Base liquid"sv, base_price, base_cap, material_class::liquid);
    auto mat3 = std::make_unique<material>("Base solid"sv, base_price, base_cap, material_class::solid);
    auto mat4 = std::make_unique<material>("Base gas"sv, 4 * base_price, base_cap, material_class::gas);

    xitren::storage::gas_liquid<xitren::storage::capacity::s> storage{};

    EXPECT_EQ(storage.capacity(), static_cast<int>(xitren::storage::capacity::s));

    storage.push(std::move(mat1));
    EXPECT_EQ(storage.load(), base_cap);

    storage.push(std::move(mat2));
    EXPECT_EQ(storage.load(), base_cap * 2);
    EXPECT_EQ(storage.used_barrels(), 2);

    storage.push(std::move(mat4));
    EXPECT_EQ(storage.used_barrels(), 2);
    auto mat5 = storage.pull("Base gas"sv, base_cap * 2);
    EXPECT_EQ(mat5->capacity(), base_cap * 2);
    EXPECT_EQ(mat5->cost(), base_price * 2.5);

    auto stat = storage.push(std::move(mat3));
    EXPECT_TRUE(stat == storage_status_type::bad_material_type);
}
