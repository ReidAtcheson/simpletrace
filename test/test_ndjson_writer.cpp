#include "acutest.h"
#include "ndjson_trace_writer.h"
#include "trace_event.h"

#include <filesystem>
#include <fstream>
#include <string>

using namespace simpletrace;

#define TEST_EVENT_FIELDS(X) X(int32_t, value)
SIMPLETRACE_EVENT_STRUCT(test_event_t, TEST_EVENT_FIELDS)

static std::span<const std::byte> as_bytes(const test_event_t &e) {
  return {reinterpret_cast<const std::byte *>(&e), sizeof(e)};
}

void test_ndjson_flush() {
  const auto &schema =
      trace_registry_t::instance().schema(test_event_t::event_id);
  const size_t bytes_for_one = sizeof(event_id_t) + schema.size + schema.align;
  auto tmp = std::filesystem::temp_directory_path() / "simpletrace_test.ndjson";
  std::filesystem::remove(tmp);
  ndjson_trace_writer_t w(tmp.string(), bytes_for_one);

  test_event_t e1{.value = 1};
  w.write(test_event_t::event_id, as_bytes(e1));
  test_event_t e2{.value = 2};
  w.write(test_event_t::event_id, as_bytes(e2));
  w.flush();

  std::ifstream in(tmp);
  std::string line;
  std::getline(in, line);
  TEST_CHECK(line.find("\"event_type\":\"test_event_t\"") != std::string::npos);
  TEST_CHECK(line.find("\"value\":1") != std::string::npos);
  std::getline(in, line);
  TEST_CHECK(line.find("\"value\":2") != std::string::npos);
  TEST_CHECK(!std::getline(in, line));
  std::filesystem::remove(tmp);
}

TEST_LIST = {{"test_ndjson_flush", test_ndjson_flush}, {NULL, NULL}};
