#pragma once
#include "trace_writer.h"

namespace simpletrace {
namespace impl {

trace_writer_t *get_tls_writer();
void set_tls_writer(trace_writer_t *writer);

} // namespace impl
} // namespace simpletrace
