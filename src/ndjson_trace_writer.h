#pragma once
#include "trace_buffer.h"
#include "trace_writer.h"

#include <fstream>
#include <string>

namespace simpletrace {

class ndjson_trace_writer_t : public trace_writer_t {
public:
  ndjson_trace_writer_t(const std::string &filename, size_t buffer_bytes);
  ~ndjson_trace_writer_t() override;

  void write(const event_id_t event, std::span<const std::byte> data) override;
  void flush() override;

private:
  void flush_buffer();
  void write_event(const trace_buffer_t::drained_event_t &e);
  static std::string escape_string(std::string_view sv);

  std::ofstream out_;
  trace_buffer_t buffer_;
};

} // namespace simpletrace
