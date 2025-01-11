
#include <xitren/unit/base_unit.h>

#include <cmath>

namespace xitren::unit {

base_unit::observer_coord_type&
base_unit::position()
{
    return coords_obs_;
}

base_unit::observer_speed_type&
base_unit::speed()
{
    return speed_obs_;
}

base_unit::observer_angle_type&
base_unit::angle()
{
    return angle_obs_;
}

base_unit::observer_coord_type&
base_unit::view_target()
{
    return view_target_obs_;
}

base_unit::observer_coord_type&
base_unit::move_target()
{
    return move_target_obs_;
}

base_unit::observer_state_type&
base_unit::state()
{
    return state_obs_;
}

void
base_unit::teleport(base_unit::coord_type const& point)
{
    auto const old = position_;
    position_      = point;
    if (old != position_) {
        coords_obs_.notify_observers(position_);
        speed_obs_.notify_observers(speed_type{position_.first - old.first, position_.second - old.second});
    }
}

void
base_unit::rotate(base_unit::angle_type const& angle)
{
    auto const old = angle_;
    angle_         = angle;
    if (old != angle_) {
        angle_obs_.notify_observers(angle_);
    }
}

void
base_unit::state(base_unit::state_use_type value)
{
    auto const old = state_;
    state_         = value;
    if (old != state_) {
        state_obs_.notify_observers(static_cast<state_type>(value.to_ulong()));
    }
}

void
base_unit::look_on(base_unit::coord_type const& point)
{
    auto const old = view_target_;
    view_target_   = point;
    if (old != view_target_) {
        view_target_obs_.notify_observers(view_target_);
        update_view(view_target_);
    }
}

void
base_unit::update_view(base_unit::coord_type const& point)
{
    auto opposite  = static_cast<double>(point.second - position_.second);
    auto adjacent  = static_cast<double>(point.first - position_.first);
    auto angle_rad = std::tan(opposite / adjacent);
    auto angle     = rad_to_deg(angle_rad);
    if (std::abs(angle_ - angle) <= parameters_.angle_speed) {
        rotate(angle);
        state_[1] = false;
        state(state_);
    } else {
        if ((angle - angle_) < 0) {
            rotate(angle_ - parameters_.angle_speed);
        } else {
            rotate(angle_ + parameters_.angle_speed);
        }
        state_[1] = true;
        state(state_);
    }
}

void
base_unit::move_to(base_unit::coord_type const& point, base_unit::speed_type const& speed)
{
    xitren::math::bezier_point<int> p0{position_.first, position_.second};
    xitren::math::bezier_point<int> p1{position_.first + speed_.first, position_.second + speed_.second};
    xitren::math::bezier_point<int> p2{point.first - speed.first, point.second - speed.second};
    xitren::math::bezier_point<int> p3{point.first, point.second};
    trajectory_.update(std::array<xitren::math::bezier_point<int>, 4>{p0, p1, p2, p3});
    traj_step_ = 0;
    update_move();
}

void
base_unit::update_move()
{
    double     speed_{};
    auto const reduced_size = trajectory_.size() - 1;
    for (; traj_step_ < reduced_size && speed_ <= parameters_.speed; traj_step_++) {
        double const x_pow = (trajectory_[traj_step_].x - trajectory_[traj_step_ + 1].x)
                             * (trajectory_[traj_step_].x - trajectory_[traj_step_ + 1].x);
        double const y_pow = (trajectory_[traj_step_].y - trajectory_[traj_step_ + 1].y)
                             * (trajectory_[traj_step_].y - trajectory_[traj_step_ + 1].y);
        double const dist = std::sqrt((x_pow) + (y_pow));
        speed_ += dist;
    }
    if (traj_step_ < reduced_size) {
        teleport(coord_type{trajectory_[traj_step_].x, trajectory_[traj_step_].y});
        state_[0] = true;
        state(state_);
    } else {
        teleport(coord_type{trajectory_.back().x, trajectory_.back().y});
        state_[0] = false;
        state(state_);
    }
}

base_unit::base_unit(base_parameters const& parameters) : parameters_{parameters} {}

void
base_unit::data(void const* src, time_type const& nd)
{
    if (state_[0]) {
        update_move();
    }
    if (state_[1]) {
        update_view(view_target_);
    }
}

constexpr int
base_unit::rad_to_deg(double radian)
{
    constexpr double coef = 180. / std::numbers::pi_v<double>;
    return static_cast<int>(radian * coef);
}

constexpr double
base_unit::deg_to_rad(int degrees)
{
    constexpr double coef = std::numbers::pi_v<double> / 180.;
    return static_cast<double>(degrees) * coef;
}

}    // namespace xitren::unit
