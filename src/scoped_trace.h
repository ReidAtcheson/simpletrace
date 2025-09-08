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
      : scoped_trace_t(writer, std::string_view(label, N - 1)) {}
  ~scoped_trace_t();

private:
  scoped_trace_t(trace_writer_t *writer, std::string_view label);
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
