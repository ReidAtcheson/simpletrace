#include "scoped_trace.h"
#include <chrono>

namespace simpletrace {
namespace {
timestamp_t now_timestamp() {
  timestamp_t ts;
  ts.dur = std::chrono::duration_cast<std::chrono::nanoseconds>(
               std::chrono::steady_clock::now().time_since_epoch())
               .count();
  return ts;
}
} // namespace

scoped_trace_t::scoped_trace_t(trace_writer_t *writer, std::string_view label)
    : writer_(writer), label_(label), start_(now_timestamp()) {
  if (writer_) {
    scope_trace_event_t ev{scope_token_t::beg, label_, start_};
    writer_->write(scope_trace_event_t::event_id,
                   std::span<const std::byte>(
                       reinterpret_cast<const std::byte *>(&ev), sizeof(ev)));
  }
}

scoped_trace_t::~scoped_trace_t() {
  if (writer_) {
    scope_trace_event_t ev{scope_token_t::end, label_, now_timestamp()};
    writer_->write(scope_trace_event_t::event_id,
                   std::span<const std::byte>(
                       reinterpret_cast<const std::byte *>(&ev), sizeof(ev)));
  }
}

} // namespace simpletrace
