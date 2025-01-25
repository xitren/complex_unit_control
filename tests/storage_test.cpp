#include <xitren/storage/containers.hpp>
#include <xitren/storage/gas_liquid.hpp>
#include <xitren/storage/solid.hpp>

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
    std::unique_ptr<xitren::unit::material> mat5 = storage.pull("Base gas"sv, base_cap * 2);
    EXPECT_EQ(mat5->capacity(), base_cap * 2);
    EXPECT_EQ(mat5->cost(), base_price * 2.5);

    auto stat = storage.push(std::move(mat3));
    EXPECT_TRUE(stat == storage_status_type::bad_material_type);
}

TEST(storage_test, solid_storage)
{
    constexpr std::size_t base_cap{100UL};
    constexpr std::size_t base_price{100UL};

    auto mat1 = std::make_unique<material>("Base solid 1"sv, base_price, base_cap, material_class::solid);
    auto mat2 = std::make_unique<material>("Base solid 2"sv, base_price, 4 * base_cap, material_class::solid);
    auto mat3 = std::make_unique<material>("Base solid 3"sv, base_price, 8 * base_cap, material_class::solid);
    auto mat4 = std::make_unique<material>("Base gas"sv, base_price, base_cap, material_class::gas);

    xitren::storage::solid<xitren::storage::capacity::s> storage{};

    auto stat = storage.push(std::move(mat4));
    EXPECT_TRUE(stat == storage_status_type::bad_material_type);

    storage.push(std::move(mat1));
    EXPECT_EQ(storage.load(), base_cap);

    storage.push(std::move(mat2));
    EXPECT_EQ(storage.load(), base_cap * 5);

    storage.push(std::move(mat3));
    EXPECT_EQ(storage.load(), base_cap * 13);

    std::unique_ptr<xitren::unit::material> mat5 = storage.pull("Base solid 1"sv, base_cap);
    EXPECT_EQ(mat5->capacity(), base_cap);
    EXPECT_EQ(mat5->cost(), base_price);

    std::unique_ptr<xitren::unit::material> mat6 = storage.pull("Base solid 2"sv, base_cap);
    EXPECT_EQ(mat6->capacity(), 4 * base_cap);
    EXPECT_EQ(mat6->cost(), base_price);

    std::unique_ptr<xitren::unit::material> mat7 = storage.pull("Base solid 3"sv, base_cap);
    EXPECT_EQ(mat7->capacity(), 8 * base_cap);
    EXPECT_EQ(mat7->cost(), base_price);
}

TEST(storage_test, containers_storage)
{
    constexpr std::size_t base_cap{100UL};
    constexpr std::size_t base_price{100UL};

    auto mat1 = std::make_unique<material>("Base container 1"sv, base_price, base_cap, material_class::container);
    auto mat2
        = std::make_unique<material>("Base container 2"sv, 8 * base_price, 4 * base_cap, material_class::container);
    auto mat3
        = std::make_unique<material>("Base container 3"sv, 16 * base_price, 8 * base_cap, material_class::container);
    auto mat4 = std::make_unique<material>("Base gas"sv, base_price, base_cap, material_class::gas);

    xitren::storage::containers<xitren::storage::capacity::s> storage{};

    auto stat = storage.push(std::move(mat4));
    EXPECT_TRUE(stat == storage_status_type::bad_material_type);

    storage.push(std::move(mat1));
    EXPECT_EQ(storage.load(), base_cap);

    storage.push(std::move(mat2));
    EXPECT_EQ(storage.load(), base_cap * 5);

    storage.push(std::move(mat3));
    EXPECT_EQ(storage.load(), base_cap * 13);

    std::unique_ptr<xitren::unit::material> mat5 = storage.pull("Base container 1"sv, base_cap);
    EXPECT_EQ(mat5->capacity(), base_cap);
    EXPECT_EQ(mat5->cost(), base_price);

    std::unique_ptr<xitren::unit::material> mat6 = storage.pull("Base container 2"sv, base_cap);
    EXPECT_EQ(mat6->capacity(), 4 * base_cap);
    EXPECT_EQ(mat6->cost(), 8 * base_price);

    std::unique_ptr<xitren::unit::material> mat7 = storage.pull("Base container 3"sv, base_cap);
    EXPECT_EQ(mat7->capacity(), 8 * base_cap);
    EXPECT_EQ(mat7->cost(), 16 * base_price);
}
