#pragma once
#include "trace_event.h"

namespace simpletrace {

#define TRACE_SCOPE_EVENT_FIELDS(X)                                            \
  X(scope_token_t, token)                                                      \
  X(std::string_view, label)                                                   \
  X(timestamp_t, timestamp)

SIMPLETRACE_EVENT_STRUCT_STATIC(scope_trace_event_t, TRACE_SCOPE_EVENT_FIELDS)

} // namespace simpletrace
