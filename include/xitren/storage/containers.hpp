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
class containers : public istore<Cap, containers> {
    using istore<Cap, containers>::load_;
    using istore<Cap, containers>::cost_;
    using istore<Cap, containers>::mean_price;

public:
    using istore<Cap, containers>::capacity;
    using istore<Cap, containers>::load;
    using istore<Cap, containers>::reset_cost;

    using status_type = storage_status_type;
    using value_type  = std::size_t;
    using mat_type    = std::unique_ptr<unit::material>;
    using name_type   = unit::material::name_type;
    using record_type = struct store {
        value_type capacity;
        value_type price;
        value_type capacity_of_one;
    };
    using storage_type = std::unordered_map<name_type, record_type>;

    int
    price() const
    {
        return price_;
    }

    status_type
    push(mat_type mat)
    {
        if (mat->type() != unit::material_class::container) {
            return status_type::bad_material_type;
        }
        if ((load() + mat->capacity()) > capacity()) {
            return status_type::storage_is_full;
        }
        if (storages_.contains(mat->name())) {
            auto has = storages_[mat->name()];
            storages_[mat->name()].capacity += mat->capacity();
            storages_[mat->name()].price = mean_price(has.price, has.capacity, mat->price(), mat->capacity());
        } else {
            storages_[mat->name()] = record_type{mat->capacity(), mat->price(), mat->capacity()};
        }
        load_ += mat->capacity();
        price_ += mat->price();
        return status_type::ok;
    }

    mat_type
    pull(unit::material::name_type const& mat_id, value_type val = 0)
    {
        if (storages_.contains(mat_id)) {
            if (storages_[mat_id].capacity >= storages_[mat_id].capacity_of_one) {
                auto& it = storages_[mat_id];
                auto  ptr
                    = unit::material(mat_id, it.price + cost_, it.capacity_of_one, unit::material_class::container);
                it.capacity -= it.capacity_of_one;
                load_ -= it.capacity_of_one;
                price_ -= it.price;
                if (it.capacity == 0) {
                    storages_.erase(mat_id);
                }
                return std::make_unique<unit::material>(ptr);
            }
        }
        return nullptr;
    }

private:
    storage_type storages_{};
    value_type   price_{};
};

}    // namespace xitren::storage
