#include "scoped_trace.h"

namespace simpletrace {
thread_local trace_writer_t *tls_writer = nullptr;
}
