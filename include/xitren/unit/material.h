
#include <xitren/comm/observer.hpp>

#include <array>
#include <atomic>

namespace xitren::unit {

class material {
    using id_type        = std::size_t;
    using time_type      = std::size_t;
    using atomic_id_type = std::atomic<std::size_t>;

public:
private:
    static inline atomic_id_type id_pool{};
    id_type                      id_{id_pool++};
    time_type                    birth_date_;
};

}    // namespace xitren::unit