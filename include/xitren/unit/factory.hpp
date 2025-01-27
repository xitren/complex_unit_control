#pragma once

#include <xitren/comm/values/observable.hpp>
#include <xitren/unit/product.hpp>

#include <array>
#include <atomic>
#include <chrono>
#include <memory>
#include <numeric>

namespace xitren::unit {

template <material_types Type>
class factory;

template <>
class factory<material_types::water> : public comm::values::observed<std::size_t> {
public:
    using value_type          = std::size_t;
    using produce_type        = product<material_types::water>;
    using unique_produce_type = std::unique_ptr<produce_type>;
    using unique_consume_type = std::unique_ptr<product<material_types::ice>>;
    using function_type       = std::function<void(unique_produce_type const)>;

    factory(value_type end_time, function_type func, unique_consume_type cons)
        : callback_{func},
          end_time_{end_time},
          cons_{std::move(cons)},
          comm::values::observed<value_type>{[&](value_type val) {
              if (val >= end_time_ && callback_ != nullptr) {
                  auto cost = (cons_->price() + cons_->cost()) / cons_->capacity();
                  for (int i{}; i < cons_->capacity(); i++) {
                      callback_(std::make_unique<produce_type>(cost));
                  }
              }
          }}
    {}

private:
    function_type       callback_{nullptr};
    value_type          end_time_{};
    unique_consume_type cons_;
};

}    // namespace xitren::unit
