#include "acutest.h"
#include "trace_event.h"

using namespace simpletrace;

#define EVENT_A_FIELDS(X) X(int32_t, value, dtype_t::i32)

SIMPLETRACE_EVENT_STRUCT(event_a_t, EVENT_A_FIELDS)

#define EVENT_B_FIELDS(X) X(int64_t, timestamp, dtype_t::timestamp)

SIMPLETRACE_EVENT_STRUCT_STATIC(event_b_t, EVENT_B_FIELDS)

void test_event_lazy() {
  event_id_t id1 = event_a_t::event_id();
  TEST_CHECK(id1 == event_a_t::event_id());
}

void test_event_eager() {
  event_id_t id1 = event_b_t::event_id;
  event_id_t id2 = trace_registry_t::instance().register_type<event_b_t>();
  TEST_CHECK(id1 == id2);
}

TEST_LIST = {{"test_event_lazy", test_event_lazy},
             {"test_event_eager", test_event_eager},
             {NULL, NULL}};
