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
    using hash_type        = std::hash<unit::material::name_type>;
    using value_type       = std::size_t;
    using mat_type         = std::unique_ptr<unit::material>;
    using gas_storage_type = struct store {
        std::size_t               hash;
        unit::material::name_type name_id;
        value_type                loaded;
        value_type                capacity;
        value_type                price;
        unit::material_class      mat_class;

        bool
        operator==(store const& other)
        {
            return (hash == other.hash) && (loaded == other.loaded) && (capacity == other.capacity)
                   && (price == other.price) && (mat_class == other.mat_class)
                   && std::equal(name_id.begin(), name_id.end(), other.name_id.begin(), other.name_id.end());
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
        auto hash       = hash_type{}(mat->name());
        auto comparator = [&](gas_storage_type& iter) {
            return iter.hash == hash
                   && std::equal(mat->name().begin(), mat->name().end(), iter.name_id.begin(), iter.name_id.end());
        };
        auto it = std::find_if(storages_.begin(), storages_.begin() + occuped_, comparator);
        if (it != (storages_.begin() + occuped_)) {
            if ((it->loaded + mat->capacity()) <= it->capacity) {
                auto mean = mean_price(it->price, it->loaded, mat->cost(), mat->capacity());
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
            auto new_item = gas_storage_type{hash,         mat->name(), mat->capacity(), capacity() / storages_.size(),
                                             mat->price(), mat->type()};
            storages_[occuped_++] = new_item;
            load_ += mat->capacity();
        }
        return status_type::ok;
    }

    mat_type
    pull(unit::material::name_type const& mat_id, value_type val)
    {
        auto hash       = hash_type{}(mat_id);
        auto comparator = [&](gas_storage_type& iter) {
            return iter.hash == hash
                   && std::equal(mat_id.begin(), mat_id.end(), iter.name_id.begin(), iter.name_id.end());
        };
        auto it = std::find_if(storages_.begin(), storages_.begin() + occuped_, comparator);
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
