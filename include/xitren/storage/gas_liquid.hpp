#pragma once

#include <xitren/comm/observer.hpp>
#include <xitren/storage/istore.hpp>

#include <xitren/storage/type.h>
#include <xitren/unit/material.h>

#include <array>
#include <bitset>
#include <string_view>

namespace xitren::storage {

template <xitren::storage::capacity Cap>
class gas_liquid : public istore<Cap, gas_liquid> {
    using istore<Cap, gas_liquid>::load_;
    using istore<Cap, gas_liquid>::cost_;

    using istore<Cap, gas_liquid>::mean_price;

public:
    using istore<Cap, gas_liquid>::capacity;
    using istore<Cap, gas_liquid>::load;
    using istore<Cap, gas_liquid>::reset_cost;

    using status_type      = storage_status_type;
    using value_type       = std::size_t;
    using mat_type         = std::unique_ptr<unit::material>;
    using gas_storage_type = struct store {
        unit::material_types id;
        value_type           loaded;
        value_type           capacity;
        value_type           price;
        unit::material_class mat_class;

        bool
        operator==(store const& other)
        {
            return (id == other.id) && (loaded == other.loaded) && (capacity == other.capacity)
                   && (price == other.price) && (mat_class == other.mat_class);
        }
    };
    using gas_storages_type = std::array<gas_storage_type, static_cast<int>(Cap) / static_cast<int>(capacity::xs)>;

    int
    barrels() const
    {
        return storages_.size();
    }

    int
    used_barrels() const
    {
        return occuped_;
    }

    status_type
    push(mat_type mat)
    {
        if ((mat->type() != unit::material_class::gas) && (mat->type() != unit::material_class::liquid)) {
            return status_type::bad_material_type;
        }
        auto comparator = [&](gas_storage_type& iter) { return mat->name() == iter.id; };
        auto it         = std::find_if(storages_.begin(), storages_.begin() + occuped_, comparator);
        if (it != (storages_.begin() + occuped_)) {
            if ((it->loaded + mat->capacity()) <= it->capacity) {
                auto mean = mean_price(it->price, it->loaded, mat->price() + mat->cost(), mat->capacity());
                it->loaded += mat->capacity();
                load_ += mat->capacity();
                it->price = mean;
                return status_type::ok;
            }
        }
        if (occuped_ >= storages_.size()) {
            if (load() == capacity()) {
                return status_type::storage_is_full;
            } else {
                return status_type::no_place_for_new_type;
            }
        }
        {
            auto new_item         = gas_storage_type{mat->name(), mat->capacity(), capacity() / storages_.size(),
                                             mat->price() + mat->cost(), mat->type()};
            storages_[occuped_++] = new_item;
            load_ += mat->capacity();
        }
        return status_type::ok;
    }

    mat_type
    pull(unit::material_types const& mat_id, value_type val)
    {
        auto comparator = [&](gas_storage_type& iter) { return mat_id == iter.id; };
        auto it         = std::find_if(storages_.begin(), storages_.begin() + occuped_, comparator);
        if (it != (storages_.begin() + occuped_)) {
            it->loaded -= val;
            auto ptr = unit::material(mat_id, it->price + cost_, val, it->mat_class);
            if (it->loaded == 0) {
                std::remove(storages_.begin(), storages_.begin() + occuped_, (*it));
                occuped_--;
            }
            return std::make_unique<unit::material>(ptr);
        }
        return nullptr;
    }

private:
    gas_storages_type storages_{};
    value_type        occuped_{};
};

}    // namespace xitren::storage
