#include "acutest.h"
#include "scoped_trace.h"
#include <vector>

using namespace simpletrace;
using namespace std::literals;

struct vector_writer_t : trace_writer_t {
  std::vector<scope_trace_event_t> events;
  void write(const event_id_t event, std::span<const std::byte> data) override {
    if (event == scope_trace_event_t::event_id) {
      events.push_back(
          *reinterpret_cast<const scope_trace_event_t *>(data.data()));
    }
  }
  void flush() override {}
};

void test_scoped_trace_basic() {
  vector_writer_t writer;
  tls_writer = &writer;
  {
    SIMPLETRACE_SCOPED_TRACE("hello"sv);
  }
  tls_writer = nullptr;
  TEST_CHECK(writer.events.size() == 2);
  TEST_CHECK(writer.events[0].token == scope_token_t::beg);
  TEST_CHECK(writer.events[1].token == scope_token_t::end);
  TEST_CHECK(writer.events[0].label == "hello"sv);
  TEST_CHECK(writer.events[1].label == "hello"sv);
  TEST_CHECK(writer.events[0].timestamp.dur <= writer.events[1].timestamp.dur);
}

TEST_LIST = {{"test_scoped_trace_basic", test_scoped_trace_basic},
             {NULL, NULL}};
