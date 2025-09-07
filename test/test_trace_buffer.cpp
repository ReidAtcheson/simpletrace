#include "acutest.h"
#include "trace_buffer.h"
#include "trace_event.h"

#include <cstring>
#include <limits>

using namespace simpletrace;

#define TEST_EVENT_FIELDS(X) X(int32_t, value)
SIMPLETRACE_EVENT_STRUCT(test_event_t, TEST_EVENT_FIELDS)

static std::span<const std::byte> as_bytes(const test_event_t &e) {
  return {reinterpret_cast<const std::byte *>(&e), sizeof(e)};
}

void test_buffer_and_drain() {
  trace_buffer_t buf(1024);

  test_event_t e1{.value = 1};
  TEST_CHECK(buf.buffer(test_event_t::event_id(), as_bytes(e1)));

  test_event_t e2{.value = 2};
  TEST_CHECK(buf.buffer(test_event_t::event_id(), as_bytes(e2)));

  auto first = buf.drain_single();
  test_event_t out1;
  std::memcpy(&out1, first.data.data(), first.data.size());
  TEST_CHECK(first.event == test_event_t::event_id());
  TEST_CHECK(out1.value == 1);
  TEST_CHECK(first.events_remaining == 1);

  auto second = buf.drain_single();
  test_event_t out2;
  std::memcpy(&out2, second.data.data(), second.data.size());
  TEST_CHECK(second.event == test_event_t::event_id());
  TEST_CHECK(out2.value == 2);
  TEST_CHECK(second.events_remaining == 0);

  auto empty = buf.drain_single();
  TEST_CHECK(empty.event == std::numeric_limits<event_id_t>::max());
  TEST_CHECK(empty.events_remaining == 0);
}

void test_buffer_overflow() {
  const auto &schema =
      trace_registry_t::instance().schema(test_event_t::event_id());
  const size_t bytes_for_one = sizeof(event_id_t) + schema.size + schema.align;
  trace_buffer_t buf(bytes_for_one);

  test_event_t e{.value = 123};
  TEST_CHECK(buf.buffer(test_event_t::event_id(), as_bytes(e)));
  TEST_CHECK(!buf.buffer(test_event_t::event_id(), as_bytes(e)));

  buf.reset();
  TEST_CHECK(buf.buffer(test_event_t::event_id(), as_bytes(e)));
}

TEST_LIST = {{"test_buffer_and_drain", test_buffer_and_drain},
             {"test_buffer_overflow", test_buffer_overflow},
             {NULL, NULL}};
