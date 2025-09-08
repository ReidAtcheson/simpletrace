#pragma once
#include "thread_local_writer.h"
#include "trace_scope_event.h"
#include "trace_writer.h"
#include <string_view>

namespace simpletrace {

namespace impl {
timestamp_t now_timestamp();
} // namespace impl

class scoped_trace_t {
public:
  template <std::size_t N>
  scoped_trace_t(trace_writer_t *writer, const char (&label)[N])
      : writer_(writer), label_(label, N - 1), start_(impl::now_timestamp()) {
    if (writer_) {
      scope_trace_event_t ev{scope_token_t::beg, label_, start_};
      writer_->write(scope_trace_event_t::event_id,
                     std::span<const std::byte>(
                         reinterpret_cast<const std::byte *>(&ev), sizeof(ev)));
    }
  }
  ~scoped_trace_t();

private:
  trace_writer_t *writer_;
  std::string_view label_;
  timestamp_t start_;
};

#define SIMPLETRACE_CONCAT_INNER(x, y) x##y
#define SIMPLETRACE_CONCAT(x, y) SIMPLETRACE_CONCAT_INNER(x, y)
#define SIMPLETRACE_SCOPED_TRACE(label)                                        \
  ::simpletrace::scoped_trace_t SIMPLETRACE_CONCAT(                            \
      __simpletrace_scoped_, __LINE__)(::simpletrace::impl::get_tls_writer(),  \
                                       label)

} // namespace simpletrace
