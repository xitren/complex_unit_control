#pragma once

#include <xitren/storage/type.h>
#include <xitren/unit/material.h>

#include <array>
#include <bitset>
#include <memory>

namespace xitren::storage {

template <xitren::storage::capacity Cap, template <xitren::storage::capacity> class Derived>
class istore {
public:
    using status_type = enum class error {
        ok,
        loadded_partually,
        storage_is_full,
        no_place_for_new_type,
        doesnt_exist,
        bad_material_type
    };

    int
    capacity() const
    {
        return static_cast<Derived<Cap> const*>(this)->capacity();
    }

    int
    load() const
    {
        return static_cast<Derived<Cap> const*>(this)->load();
    }

    status_type
    push(std::unique_ptr<unit::material> mat)
    {
        return static_cast<Derived<Cap> const*>(this)->push(std::move(mat));
    }

    status_type
    pull(unit::material::name_type const& mat_id)
    {
        return static_cast<Derived<Cap> const*>(this)->pull(mat_id);
    }

private:
};

}    // namespace xitren::storage
