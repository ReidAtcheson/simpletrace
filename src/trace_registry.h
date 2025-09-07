#pragma once
#include <cstdint>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace simpletrace {

using event_id_t = uint16_t;

enum class dtype_t { i32, i64, f32, f64, timestamp };

struct timestamp_t {
  int64_t value;
};

template <typename T> struct dtype_of;

template <> struct dtype_of<int32_t> {
  static constexpr dtype_t value = dtype_t::i32;
};
template <> struct dtype_of<int64_t> {
  static constexpr dtype_t value = dtype_t::i64;
};
template <> struct dtype_of<float> {
  static constexpr dtype_t value = dtype_t::f32;
};
template <> struct dtype_of<double> {
  static constexpr dtype_t value = dtype_t::f64;
};
template <> struct dtype_of<timestamp_t> {
  static constexpr dtype_t value = dtype_t::timestamp;
};

struct field_layout_t {
  std::string_view name;
  dtype_t dtype;
  size_t offset;
  size_t size;
};

struct schema_t {
  std::string_view name;
  event_id_t event_id;
  size_t size;
  size_t align;
  /*We could optionally make this a std::array
   * of large-ish size to avoid heap allocation*/
  std::vector<field_layout_t> fields;
};

class trace_registry_t {
public:
  static trace_registry_t &instance();
  event_id_t register_index_schema(const std::type_index ti,
                                   const schema_t &schema);
  template <typename T> event_id_t register_type() {
    return register_index_schema(std::type_index(typeid(T)), T::schema());
  }

private:
  std::unordered_map<std::type_index, event_id_t> events_;
  std::vector<schema_t> schemas_;
};

} // namespace simpletrace
