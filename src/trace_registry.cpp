#include "trace_registry.h"

#include <cassert>

namespace simpletrace {


timestamp_t::timepoint_t timestamp_t::timepoint() const noexcept{
  return timepoint_t(timepoint_t::duration(dur));
}

trace_registry_t &trace_registry_t::instance() {
  static trace_registry_t r;
  return r;
}

const schema_t& trace_registry_t::schema(const event_id_t id) const{
  assert(id<schemas_.size());
  return schemas_[id];
}

event_id_t trace_registry_t::register_index_schema(const std::type_index ti,
                                                   const schema_t &schema) {
  event_id_t next_event = static_cast<event_id_t>(schemas_.size());
  if (auto &&[it, inserted] = events_.emplace(ti, next_event); !inserted) {
    return it->second;
  }
  schemas_.emplace_back(schema);
  return next_event;
}
} // namespace simpletrace
