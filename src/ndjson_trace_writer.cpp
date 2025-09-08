#include "ndjson_trace_writer.h"

#include <cassert>
#include <limits>
#include <string_view>

namespace simpletrace {

ndjson_trace_writer_t::ndjson_trace_writer_t(const std::string &filename,
                                             size_t buffer_bytes)
    : out_(filename), buffer_(buffer_bytes) {}

ndjson_trace_writer_t::~ndjson_trace_writer_t() { flush(); }

void ndjson_trace_writer_t::write(const event_id_t event,
                                  std::span<const std::byte> data) {
  if (!buffer_.buffer(event, data)) {
    flush_buffer();
    buffer_.reset();
    bool buffered = buffer_.buffer(event, data);
    assert(buffered);
  }
}

void ndjson_trace_writer_t::flush() {
  flush_buffer();
  out_.flush();
}

void ndjson_trace_writer_t::flush_buffer() {
  while (true) {
    auto e = buffer_.drain_single();
    if (e.event == std::numeric_limits<event_id_t>::max())
      break;
    write_event(e);
  }
}

void ndjson_trace_writer_t::write_event(
    const trace_buffer_t::drained_event_t &e) {
  const auto &schema = trace_registry_t::instance().schema(e.event);
  out_ << "{\"event_type\":\"" << schema.name << "\"";
  const std::byte *base = e.data.data();
  for (const auto &field : schema.fields) {
    out_ << ",\"" << field.name << "\":";
    const std::byte *ptr = base + field.offset;
    switch (field.dtype) {
    case dtype_t::i32:
      out_ << *reinterpret_cast<const int32_t *>(ptr);
      break;
    case dtype_t::i64:
      out_ << *reinterpret_cast<const int64_t *>(ptr);
      break;
    case dtype_t::f32:
      out_ << *reinterpret_cast<const float *>(ptr);
      break;
    case dtype_t::f64:
      out_ << *reinterpret_cast<const double *>(ptr);
      break;
    case dtype_t::timestamp:
      out_ << reinterpret_cast<const timestamp_t *>(ptr)->dur;
      break;
    case dtype_t::string_view: {
      auto sv = *reinterpret_cast<const std::string_view *>(ptr);
      out_ << "\"" << escape_string(sv) << "\"";
      break;
    }
    case dtype_t::scope_token: {
      auto tok = *reinterpret_cast<const scope_token_t *>(ptr);
      out_ << (tok == scope_token_t::beg ? "\"beg\"" : "\"end\"");
      break;
    }
    }
  }
  out_ << "}\n";
}

std::string ndjson_trace_writer_t::escape_string(std::string_view sv) {
  std::string out;
  out.reserve(sv.size());
  for (char c : sv) {
    if (c == '"' || c == '\\')
      out.push_back('\\');
    out.push_back(c);
  }
  return out;
}

} // namespace simpletrace
