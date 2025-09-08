#include "scoped_trace.h"
#include <chrono>

namespace simpletrace {
namespace impl {
timestamp_t now_timestamp() {
  timestamp_t ts;
  ts.dur = std::chrono::duration_cast<std::chrono::nanoseconds>(
               std::chrono::steady_clock::now().time_since_epoch())
               .count();
  return ts;
}
} // namespace impl

scoped_trace_t::~scoped_trace_t() {
  if (writer_) {
    scope_trace_event_t ev{scope_token_t::end, label_, impl::now_timestamp()};
    writer_->write(scope_trace_event_t::event_id,
                   std::span<const std::byte>(
                       reinterpret_cast<const std::byte *>(&ev), sizeof(ev)));
  }
}

} // namespace simpletrace
