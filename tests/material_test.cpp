#include <gtest/gtest.h>
#include <xitren/unit/material.h>
#include <xitren/unit/material_complex.h>

#include <algorithm>
#include <array>
#include <iostream>

using namespace xitren::unit;

TEST(material_test, base)
{
    material mat1{};
    material mat2{};
    material mat3{};

    mat1.notification(nullptr, 1);
    mat2.notification(nullptr, 1);
    mat2.notification(nullptr, 1);

    EXPECT_TRUE(mat1.cost() == 101);
    EXPECT_TRUE(mat2.cost() == 102);
    EXPECT_TRUE(mat3.cost() == 100);
}

TEST(material_test, complex)
{
    material mat1{};
    material mat2{};
    material mat3{};

    mat1.notification(nullptr, 1);
    mat2.notification(nullptr, 1);
    mat2.notification(nullptr, 1);

    EXPECT_TRUE(mat1.cost() == 101);
    EXPECT_TRUE(mat2.cost() == 102);
    EXPECT_TRUE(mat3.cost() == 100);

    material_complex<3> mat_c("Complex material", 100, std::array<material, 3>{{mat1, mat2, mat3}});

    EXPECT_EQ(mat_c.cost(), (100 + 101 + 102 + 100));
}
