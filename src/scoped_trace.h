#pragma once
#include "thread_local_writer.h"
#include "trace_scope_event.h"
#include "trace_writer.h"
#include <string_view>

namespace simpletrace {

class scoped_trace_t {
public:
  scoped_trace_t(trace_writer_t *writer, std::string_view label);
  ~scoped_trace_t();

private:
  trace_writer_t *writer_;
  std::string_view label_;
};

#define SIMPLETRACE_CONCAT_INNER(x, y) x##y
#define SIMPLETRACE_CONCAT(x, y) SIMPLETRACE_CONCAT_INNER(x, y)
#define SIMPLETRACE_SCOPED_TRACE(label)                                        \
  ::simpletrace::scoped_trace_t SIMPLETRACE_CONCAT(                            \
      __simpletrace_scoped_, __LINE__)(::simpletrace::impl::get_tls_writer(),  \
                                       label)

} // namespace simpletrace
