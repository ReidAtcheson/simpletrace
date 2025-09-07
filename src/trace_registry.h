#pragma once
#include <cstdint>
#include <vector>
#include <string>


namespace simpletrace{

using event_id_t = uint16_t;

enum class dtype_t{
  i32,
  i64,
  f32,
  f64,
  timestamp
};

struct field_layout_t{
  std::string_view name;
  dtype_t dtype;
  size_t offset;
  size_t size;
};

struct schema_t {
  std::string_view name;
  event_id_t event_id;
  size_t size;
  std::vector<field_layout_t> fields;
};

class trace_registry_t{

};

}
