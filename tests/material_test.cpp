#include <xitren/comm/values/observable.hpp>
#include <xitren/unit/factory.hpp>

#include <gtest/gtest.h>
#include <xitren/unit/material.h>

#include <algorithm>
#include <array>
#include <iostream>
#include <string_view>

using namespace xitren::unit;
using namespace std::literals;

TEST(material_test, base)
{
    auto mat1 = std::make_unique<material>();
    auto mat2 = std::make_unique<material>();
    auto mat3 = std::make_unique<material>();

    mat1->notification(nullptr, 1);
    mat2->notification(nullptr, 1);
    mat2->notification(nullptr, 1);

    EXPECT_EQ(mat1->cost(), 1);
    EXPECT_EQ(mat2->cost(), 2);
    EXPECT_EQ(mat3->cost(), 0);
}

TEST(material_test, factory)
{
    int                                              i{};
    xitren::comm::values::observable<std::size_t, 1> ticks{};
    auto                                             mat1 = std::make_unique<product<material_types::ice>>(308, 154);

    factory<material_types::water> fact(
        5,
        [&](std::unique_ptr<product<material_types::water>> prod) {
            std::cout << "Generated " << prod->price() << std::endl;
            i++;
        },
        std::move(mat1));

    ticks.add_observer(fact);
    ticks = 5;
    EXPECT_EQ(i, 154);
}
