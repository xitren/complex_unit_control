#pragma once

#include <sys/wait.h>
#include <xitren/unit/material.h>

#include <array>
#include <bitset>

namespace xitren::storage {

template <class T>
class istore {
public:
    using status_type = enum class error {
        ok,
        loadded_partually,
        storage_is_full,
        no_place_for_new_type,
        doesnt_exist
    };

    int
    capacity()
    {
        return static_cast<T const*>(this)->capacity();
    }

    int
    load()
    {
        return static_cast<T const*>(this)->load();
    }

    status_type
    push(std::unique_ptr<unit::material> mat)
    {
        return static_cast<T const*>(this)->push(mat);
    }

    status_type
    pull(unit::material::name_type const& mat_id)
    {
        return static_cast<T const*>(this)->pull(mat_id);
    }
};

}    // namespace xitren::storage
