
#include <xitren/unit/material.h>

#include <cmath>

namespace xitren::unit {

material::id_type
material::id() const
{
    return id_;
}

material::time_type
material::age() const
{
    return now_in_ms() - birth_date_;
}

material::price_type
material::price() const
{
    return base_price;
}

material::price_type
material::cost() const
{
    return price_;
}

void
material::data(void const* src, price_type const& nd)
{
    price_ += nd;
}

material::time_type
material::now_in_ms() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
        .count();
}

}    // namespace xitren::unit