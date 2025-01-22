
#include <xitren/storage/energy.h>

#include <cmath>
using namespace std::chrono;

namespace xitren::storage {

energy_consumer::energy_consumer(energy_val_type consumed) : consumed_{consumed} {}

energy_consumer::energy_mode_obs_type&
energy_consumer::state()
{
    return state_obs_;
}

void
energy_consumer::state(energy_state st)
{
    auto const old = state_;
    state_         = st;
    if (old != state_) {
        state_obs_.notify_observers(state_);
    }
}

void
energy_consumer::src(energy* src)
{
    src_ = src;
}

energy_consumer::energy_val_type
energy_consumer::consume() const
{
    return consumed_;
}

energy::energy(capacity max, energy_val_type regen)
    : max_{static_cast<int>(max)}, regenerated_{regen}, func::interval_event([&]() { cycle(); }, 1000ms, 1ms)
{}

void
energy::cycle()
{
    int i{regenerated_};
    for (auto* cons : consumers_) {
        i -= cons->consume();
    }
    if ((i < 0) && (stored_ <= (-i))) {
        i       = regenerated_;
        auto it = consumers_.begin();
        for (; it != consumers_.end();) {
            if (i <= (*it)->consume()) {
                break;
            }
            i -= (*it)->consume();
            (*it)->state(energy_state::online);
            it++;
        }
        for (; it != consumers_.end();) {
            (*it)->state(energy_state::offline);
            it++;
        }
    } else {
        for (auto* cons : consumers_) {
            cons->state(energy_state::online);
        }
    }
    stored(std::min(max_, stored_ + i));
}

void
energy::stored(energy_val_type val)
{
    auto const old = stored_;
    stored_        = val;
    if (old != stored_) {
        stored_obs_.notify_observers(stored_);
    }
}

energy::energy_val_type
energy::regenerated() const
{
    return regenerated_;
}

energy::energy_val_type
energy::max() const
{
    return max_;
}

energy::energy_obs_type&
energy::stored()
{
    return stored_obs_;
}

void
energy::connect(energy_consumer& cons)
{
    consumers_.emplace(&cons);
    cons.src(this);
    cons.state(energy_state::online);
}

void
energy::disconnect(energy_consumer& cons)
{
    if (consumers_.contains(&cons)) {
        consumers_.erase(&cons);
        cons.src(nullptr);
        cons.state(energy_state::offline);
    }
}

}    // namespace xitren::storage
