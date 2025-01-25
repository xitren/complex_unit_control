#include <gtest/gtest.h>
#include <xitren/unit/material.h>
#include <xitren/unit/material_complex.h>

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

    EXPECT_TRUE(mat1->cost() == 101);
    EXPECT_TRUE(mat2->cost() == 102);
    EXPECT_TRUE(mat3->cost() == 100);
}

TEST(material_test, complex)
{
    auto mat1 = std::make_unique<material>();
    auto mat2 = std::make_unique<material>();
    auto mat3 = std::make_unique<material>();

    mat1->notification(nullptr, 1);
    mat2->notification(nullptr, 1);
    mat2->notification(nullptr, 1);

    EXPECT_TRUE(mat1->cost() == 101);
    EXPECT_TRUE(mat2->cost() == 102);
    EXPECT_TRUE(mat3->cost() == 100);

    auto mat_c = std::make_unique<material_complex<3>>(
        "Complex material"sv, 100, std::array<material::price_type, 3>{{mat1->cost(), mat2->cost(), mat3->cost()}});

    EXPECT_EQ(mat_c->cost(), (100 + 101 + 102 + 100));
}
