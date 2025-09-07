#include "trace_registry.h"


namespace simpletrace{


trace_registry_t& trace_registry_t::instance(){
  static trace_registry_t r;
  return r;
}

event_id_t trace_registry_t::register_index_schema(const std::type_index ti,const schema_t& schema){
  event_id_t next_event = static_cast<event_id_t>(schemas_.size());
  if(auto&& [it,inserted] = events_.emplace(ti,next_event); !inserted){
    return it->second;
  }
  schemas_.emplace_back(schema);
  return next_event;
}
}
