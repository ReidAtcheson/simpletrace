#include "thread_local_writer.h"

namespace simpletrace {
namespace impl {

thread_local trace_writer_t *tls_writer = nullptr;

trace_writer_t *get_tls_writer() { return tls_writer; }
void set_tls_writer(trace_writer_t *writer) { tls_writer = writer; }

} // namespace impl
} // namespace simpletrace
