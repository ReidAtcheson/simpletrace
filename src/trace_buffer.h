#pragma once
#include "trace_registry.h"

#include <cstddef>
#include <memory_resource>
#include <span>
#include <vector>

namespace simpletrace {

/*
 * A generic in-memory buffer to be used by actual trace writers
 */
class trace_buffer_t {
public:
  struct drained_event_t {
    event_id_t event;
    std::span<const std::byte> data;
    size_t events_remaining;
  };
  trace_buffer_t(const size_t bytes);
  [[nodiscard]] bool buffer(const event_id_t event,
                            std::span<const std::byte> data) noexcept;
  drained_event_t drain_single();

  void reset();

private:
  size_t nevents_ = 0;
  std::vector<std::byte> data_;
  std::pmr::monotonic_buffer_resource mr_;
  std::pmr::polymorphic_allocator<std::byte> pa_;

  /*Recover data by grabbing pointers out of the same buffer in the same
   * order we allocated them.*/
  std::pmr::monotonic_buffer_resource drain_mr_;
  std::pmr::polymorphic_allocator<std::byte> drain_pa_;
};

} // namespace simpletrace
