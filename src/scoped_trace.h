#pragma once
#include "trace_scope_event.h"
#include "trace_writer.h"
#include <chrono>
#include <string_view>

namespace simpletrace {

extern thread_local trace_writer_t *tls_writer;

inline timestamp_t now_timestamp() {
  timestamp_t ts;
  ts.dur = std::chrono::duration_cast<std::chrono::nanoseconds>(
               std::chrono::steady_clock::now().time_since_epoch())
               .count();
  return ts;
}

class scoped_trace_t {
public:
  scoped_trace_t(trace_writer_t *writer, std::string_view label)
      : writer_(writer), label_(label), start_(now_timestamp()) {
    if (writer_) {
      scope_trace_event_t ev{scope_token_t::beg, label_, start_};
      writer_->write(scope_trace_event_t::event_id,
                     std::span<const std::byte>(
                         reinterpret_cast<const std::byte *>(&ev), sizeof(ev)));
    }
  }
  ~scoped_trace_t() {
    if (writer_) {
      scope_trace_event_t ev{scope_token_t::end, label_, now_timestamp()};
      writer_->write(scope_trace_event_t::event_id,
                     std::span<const std::byte>(
                         reinterpret_cast<const std::byte *>(&ev), sizeof(ev)));
    }
  }

private:
  trace_writer_t *writer_;
  std::string_view label_;
  timestamp_t start_;
};

#define SIMPLETRACE_CONCAT_INNER(x, y) x##y
#define SIMPLETRACE_CONCAT(x, y) SIMPLETRACE_CONCAT_INNER(x, y)
#define SIMPLETRACE_SCOPED_TRACE(label)                                        \
  ::simpletrace::scoped_trace_t SIMPLETRACE_CONCAT(                            \
      __simpletrace_scoped_, __LINE__)(::simpletrace::tls_writer, label)

} // namespace simpletrace
