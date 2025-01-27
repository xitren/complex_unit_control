#pragma once

#include <xitren/comm/observer.hpp>
#include <xitren/storage/istore.hpp>

#include <xitren/storage/type.h>
#include <xitren/unit/material.h>

#include <bitset>
#include <string_view>
#include <vector>

namespace xitren::storage {

template <xitren::storage::capacity Cap>
class solid : public istore<Cap, solid> {
    using istore<Cap, solid>::load_;
    using istore<Cap, solid>::cost_;
    using istore<Cap, solid>::mean_price;

public:
    using istore<Cap, solid>::capacity;
    using istore<Cap, solid>::load;
    using istore<Cap, solid>::reset_cost;

    using status_type  = storage_status_type;
    using hash_type    = std::hash<unit::material::name_type>;
    using value_type   = std::size_t;
    using mat_type     = std::unique_ptr<unit::material>;
    using storage_type = std::vector<mat_type>;

    int
    price() const
    {
        return price_;
    }

    status_type
    push(mat_type mat)
    {
        if (mat->type() != unit::material_class::solid) {
            return status_type::bad_material_type;
        }
        if ((load() + mat->capacity()) > capacity()) {
            return status_type::storage_is_full;
        }
        // this->add_observer(*mat);
        load_ += mat->capacity();
        price_ += mat->price();
        storages_.emplace_back(std::move(mat));
        return status_type::ok;
    }

    mat_type
    pull(unit::material::name_type const& mat_id, value_type val = 0)
    {
        auto hash       = hash_type{}(mat_id);
        auto comparator = [&](mat_type& ptr) { return hash_type{}(ptr->name()) == hash && mat_id == ptr->name(); };
        auto it         = std::find_if(storages_.begin(), storages_.end(), comparator);
        if (it != (storages_.end())) {
            load_ -= (*it)->capacity();
            price_ -= (*it)->price();
            auto ptr = unit::material(mat_id, (*it)->price() + (*it)->cost() + cost_, (*it)->capacity(),
                                      unit::material_class::solid);
            std::remove(storages_.begin(), storages_.end(), (*it));
            return std::make_unique<unit::material>(ptr);
        }
        return nullptr;
    }

private:
    storage_type storages_{};
    value_type   price_{};
};

}    // namespace xitren::storage
