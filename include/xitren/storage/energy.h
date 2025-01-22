#pragma once

#include <xitren/comm/observer.hpp>
#include <xitren/func/interval_event.hpp>

#include <xitren/storage/type.h>

#include <array>
#include <bitset>
#include <set>

namespace xitren::storage {

enum class energy_state { online, offline };

class energy;

class energy_consumer {
public:
    using energy_val_type      = int;
    using energy_mode_obs_type = xitren::comm::observable_dynamic<energy_state>;

    explicit energy_consumer(energy_val_type consumed);

    void
    state(energy_state st);

    void
    src(energy* src);

    energy_val_type
    consume() const;

    energy_mode_obs_type&
    state();

private:
    energy_val_type      consumed_{};
    energy_state         state_{energy_state::offline};
    energy*              src_{nullptr};
    energy_mode_obs_type state_obs_{};
};

class energy : func::interval_event {
public:
    using energy_val_type = int;
    using consumers_type  = std::set<energy_consumer*>;
    using energy_obs_type = xitren::comm::observable_dynamic<energy_val_type>;

    explicit energy(capacity max, energy_val_type regen);

    energy_val_type
    regenerated() const;
    energy_val_type
    max() const;
    energy_obs_type&
    stored();

    void
    connect(energy_consumer& cons);
    void
    disconnect(energy_consumer& cons);

private:
    energy_val_type regenerated_{};
    energy_val_type max_{};
    energy_val_type stored_{};

    consumers_type  consumers_{};
    energy_obs_type stored_obs_{};

    void
    cycle();

    void
    stored(energy_val_type val);
};

}    // namespace xitren::storage
