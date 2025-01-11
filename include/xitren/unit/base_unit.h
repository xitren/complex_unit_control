#pragma once

#include <xitren/comm/observer.hpp>
#include <xitren/math/bezier.hpp>

#include <array>
#include <bitset>

namespace xitren::unit {

using tick_time_follower_type = xitren::comm::observer<std::size_t>;

struct base_parameters {
    int speed;
    int angle_speed;
};

class base_unit : public tick_time_follower_type {
public:
    using coord_type = std::pair<int, int>;
    using speed_type = std::pair<int, int>;
    using angle_type = int;

    using observer_coord_type = xitren::comm::observable_dynamic<coord_type>;
    using observer_speed_type = xitren::comm::observable_dynamic<speed_type>;
    using observer_angle_type = xitren::comm::observable_dynamic<angle_type>;

    using time_type = std::size_t;

    enum class state_type : unsigned int { idle = 0, moving = 1, rotating = 2, moving_rotating = 3 };
    using state_use_type      = std::bitset<2>;
    using observer_state_type = xitren::comm::observable_dynamic<state_type>;

    static constexpr int steps = 100;
    using trajectory_type      = xitren::math::bezier_quadratic<int, steps>;

    observer_coord_type&
    position();
    observer_speed_type&
    speed();
    observer_angle_type&
    angle();
    observer_coord_type&
    view_target();
    observer_coord_type&
    move_target();
    observer_state_type&
    state();

    void
    teleport(coord_type const&);
    void
    move_to(coord_type const&, speed_type const&);
    void
    rotate(angle_type const&);
    void
    look_on(coord_type const&);

    explicit base_unit(base_parameters const& parameters);

protected:
    void
    data(void const* src, time_type const& nd) override;

    void
    state(state_use_type const&);
    void
    speed(speed_type const&);

private:
    coord_type     position_{};
    speed_type     speed_{};
    angle_type     angle_{};
    coord_type     view_target_{};
    coord_type     move_target_{};
    speed_type     speed_target_{};
    state_use_type state_{};
    state_type     state_val_{state_type::idle};

    base_parameters parameters_{};

    observer_coord_type coords_obs_{};
    observer_speed_type speed_obs_{};
    observer_angle_type angle_obs_{};
    observer_coord_type view_target_obs_{};
    observer_coord_type move_target_obs_{};
    observer_state_type state_obs_{};

    time_type time_{};

    trajectory_type trajectory_{};
    int             traj_step_{};

    static constexpr int
    rad_to_deg(double);
    static constexpr double
    deg_to_rad(int);

    void
    update_view(coord_type const& point);
    void
    update_move();
};

}    // namespace xitren::unit
