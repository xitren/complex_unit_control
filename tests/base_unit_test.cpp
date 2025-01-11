#include <gtest/gtest.h>
#include <xitren/unit/base_unit.h>

#include <algorithm>
#include <array>
#include <iostream>

using namespace xitren::unit;

static constexpr int max_loops{100};

/**
 * @brief Check teleport
 *               ^
 *               |
 *       1       |       2
 *               |
 *               |
 *---------------0-------------->
 *               |
 *               |
 *       3       |       4
 *               |
 */
TEST(base_unit_test, test_teleport)
{
    struct check_upd : xitren::comm::observer<base_unit::coord_type> {
        base_unit::coord_type value{};

    protected:
        void
        data(void const* src, base_unit::coord_type const& nd) override
        {
            value = nd;
            std::cout << "Teleport to location point {" << value.first << ", " << value.second << "}\n";
        }
    } checker{};

    base_unit unit{base_parameters{1, 2}};
    unit.position().add_observer(checker);

    std::array<base_unit::coord_type, 5> points{{{0, 0}, {-7, 7}, {7, 7}, {-7, -7}, {7, -7}}};
    for (int i{1}; i < points.size(); i++) {
        unit.teleport(points[i]);
        EXPECT_TRUE(checker.value == points[i]);
    }
}

/**
 * @brief Check rotate
 *               ^
 *               |
 *       1       |       2
 *               |
 *               |
 *---------------0-------------->
 *               |
 *               |
 *       3       |       4
 *               |
 */
TEST(base_unit_test, test_rotate)
{
    struct check_upd : xitren::comm::observer<base_unit::angle_type> {
        base_unit::angle_type value{};

    protected:
        void
        data(void const* src, base_unit::angle_type const& nd) override
        {
            value = nd;
            std::cout << "Rotate to angle {" << value << "}\n";
        }
    } checker{};

    base_unit unit{base_parameters{1, 2}};
    unit.angle().add_observer(checker);

    std::array<base_unit::angle_type, 4> angles{{-45, 45, 135, -135}};
    for (int i{}; i < angles.size(); i++) {
        unit.rotate(angles[i]);
        EXPECT_TRUE(checker.value == angles[i]);
    }
}

/**
 * @brief Check look_on
 *               ^
 *               |
 *       1       |       2
 *               |
 *               |
 *---------------0-------------->
 *               |
 *               |
 *       3       |       4
 *               |
 */
TEST(base_unit_test, test_look_on)
{
    struct check_upd : xitren::comm::observer<base_unit::angle_type>, xitren::comm::observer<base_unit::state_type> {
        base_unit::angle_type value{};
        base_unit::state_type state{};

    protected:
        void
        data(void const* src, base_unit::angle_type const& nd) override
        {
            value = nd;
            std::cout << "Rotate to angle {" << value << "}\n";
        }

        void
        data(void const* src, base_unit::state_type const& nd) override
        {
            state = nd;
            std::cout << "State changed {" << static_cast<unsigned int>(state) << "}\n";
        }
    } checker{};

    base_unit unit{base_parameters{1, 2}};
    unit.angle().add_observer(checker);
    unit.state().add_observer(checker);

    std::array<base_unit::angle_type, 5> angles{{0, 135, 45, -135, -45}};
    std::array<base_unit::coord_type, 5> points{{{0, 0}, {-7, 7}, {7, 7}, {-7, -7}, {7, -7}}};
    for (int i{1}; i < points.size(); i++) {
        unit.look_on(points[i]);
        for (int l{}; l < max_loops && checker.state != base_unit::state_type::idle; l++) {
            unit.notification(nullptr, l);
        }
        EXPECT_TRUE(checker.value == angles[i]);
    }
}

/**
 * @brief Check move_to
 *               ^
 *               |
 *       1       |       2
 *               |
 *               |
 *---------------0-------------->
 *               |
 *               |
 *       3       |       4
 *               |
 */
TEST(base_unit_test, test_move_to)
{
    struct check_upd : xitren::comm::observer<base_unit::coord_type>, xitren::comm::observer<base_unit::state_type> {
        base_unit::coord_type value{};
        base_unit::state_type state{};

    protected:
        void
        data(void const* src, base_unit::coord_type const& nd) override
        {
            value = nd;
            std::cout << "Teleport to location point {" << value.first << ", " << value.second << "}\n";
        }

        void
        data(void const* src, base_unit::state_type const& nd) override
        {
            state = nd;
            std::cout << "State changed {" << static_cast<unsigned int>(state) << "}\n";
        }
    } checker{};

    struct check_upd_speed : xitren::comm::observer<base_unit::speed_type> {
        base_unit::speed_type speed{};

    protected:
        void
        data(void const* src, base_unit::speed_type const& nd) override
        {
            speed = nd;
            std::cout << "Current speed {" << speed.first << ", " << speed.second << "}\n";
        }
    } checker_speed{};

    base_unit unit{base_parameters{1, 2}};
    unit.position().add_observer(checker);
    unit.state().add_observer(checker);
    unit.speed().add_observer(checker_speed);

    std::array<base_unit::coord_type, 5> points{{{0, 0}, {-7, 7}, {7, 7}, {-7, -7}, {7, -7}}};
    for (int i{1}; i < points.size(); i++) {
        unit.move_to(points[i], base_unit::speed_type{0, 0});
        for (int l{}; l < max_loops && checker.state != base_unit::state_type::idle; l++) {
            unit.notification(nullptr, l);
        }
        EXPECT_TRUE(checker.value == points[i]);
    }
}
