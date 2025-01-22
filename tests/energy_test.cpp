#include <gtest/gtest.h>
#include <xitren/storage/energy.h>

#include <algorithm>
#include <array>
#include <iostream>

using namespace xitren::storage;

TEST(energy_test, loading_battery)
{
    struct check_upd : xitren::comm::observer<energy::energy_val_type> {
        energy::energy_val_type value{};

    protected:
        void
        data(void const* src, energy::energy_val_type const& nd) override
        {
            value = nd;
            std::cout << "Energy {" << value << "}\n";
        }
    } checker{};

    energy gen{capacity::s, 10'000};
    gen.stored().add_observer(checker);

    int i{};
    for (i = 0; checker.value < gen.max(); i++) {
        std::this_thread::sleep_for(1s);
    }
    std::cout << std::endl;
    EXPECT_TRUE(i > 5);
    EXPECT_TRUE(static_cast<int>(capacity::s) == checker.value);
}

TEST(energy_test, loading_and_spent)
{
    struct check_upd : xitren::comm::observer<energy::energy_val_type> {
        energy::energy_val_type value{};

    protected:
        void
        data(void const* src, energy::energy_val_type const& nd) override
        {
            value = nd;
            std::cout << "Energy {" << value << "}\n";
        }
    } checker{};

    energy gen{capacity::s, 10'000};
    gen.stored().add_observer(checker);

    int i{};
    for (i = 0; checker.value < gen.max(); i++) {
        std::this_thread::sleep_for(1s);
    }
    std::cout << std::endl;
    EXPECT_TRUE(i > 5);
    EXPECT_TRUE(static_cast<int>(capacity::s) == checker.value);

    struct check_on1 : xitren::comm::observer<energy_state> {
        energy_state value{};

    protected:
        void
        data(void const* src, energy_state const& nd) override
        {
            value = nd;
            if (value == energy_state::online) {
                std::cout << "Connected 1 state { online }\n";
            } else {
                std::cout << "Connected 1 state { offline }\n";
            }
        }
    } on1{};

    struct check_on2 : xitren::comm::observer<energy_state> {
        energy_state value{};

    protected:
        void
        data(void const* src, energy_state const& nd) override
        {
            value = nd;
            if (value == energy_state::online) {
                std::cout << "Connected 2 state { online }\n";
            } else {
                std::cout << "Connected 2 state { offline }\n";
            }
        }
    } on2{};

    energy_consumer cons1(10'000);
    energy_consumer cons2(10'000);

    cons1.state().add_observer(on1);
    cons2.state().add_observer(on2);

    gen.connect(cons1);
    gen.connect(cons2);
    for (i = 0; on2.value == energy_state::online; i++) {
        std::this_thread::sleep_for(1s);
    }
    std::cout << std::endl;
    EXPECT_TRUE(i > 5);
    EXPECT_TRUE(on2.value == energy_state::offline);
}
