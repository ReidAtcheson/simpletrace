#include "acutest.h"
#include "trace_event.h"

using namespace simpletrace;

#define EVENT_B_FIELDS(X) X(timestamp_t, timestamp)

SIMPLETRACE_EVENT_STRUCT(event_b_t, EVENT_B_FIELDS)

void test_event_static() {
  event_id_t id1 = event_b_t::event_id;
  event_id_t id2 = trace_registry_t::instance().register_type<event_b_t>();
  TEST_CHECK(id1 == id2);
}

TEST_LIST = {{"test_event_static", test_event_static}, {NULL, NULL}};
