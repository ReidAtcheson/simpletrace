#pragma once
#include <cstddef>
#include <optional>

#include "trace_registry.h"

#define SIMPLETRACE_FIELD_DECL(type, name) type name;
#define SIMPLETRACE_FIELD_LAYOUT(type, name)                                   \
  ::simpletrace::field_layout_t{#name, ::simpletrace::dtype_of<type>::value,   \
                                offsetof(__trace_this_t, name), sizeof(type)},
#define SIMPLETRACE_EVENT_STRUCT(name, FIELDS)                                 \
  struct name {                                                                \
    FIELDS(SIMPLETRACE_FIELD_DECL)                                             \
    static ::simpletrace::schema_t schema() {                                  \
      using __trace_this_t = name;                                             \
      static const ::simpletrace::schema_t s{                                  \
          #name,                                                               \
          0,                                                                   \
          sizeof(__trace_this_t),                                              \
          alignof(__trace_this_t),                                             \
          {FIELDS(SIMPLETRACE_FIELD_LAYOUT)}};                                 \
      return s;                                                                \
    }                                                                          \
    static ::simpletrace::event_id_t event_id() {                              \
      static std::optional<::simpletrace::event_id_t> cached;                  \
      if (!cached) {                                                           \
        cached =                                                               \
            ::simpletrace::trace_registry_t::instance().register_type<name>(); \
      }                                                                        \
      return *cached;                                                          \
    }                                                                          \
  };

#define SIMPLETRACE_EVENT_STRUCT_STATIC(name, FIELDS)                          \
  struct name {                                                                \
    FIELDS(SIMPLETRACE_FIELD_DECL)                                             \
    static ::simpletrace::schema_t schema() {                                  \
      using __trace_this_t = name;                                             \
      static const ::simpletrace::schema_t s{                                  \
          #name,                                                               \
          0,                                                                   \
          sizeof(__trace_this_t),                                              \
          alignof(__trace_this_t),                                             \
          {FIELDS(SIMPLETRACE_FIELD_LAYOUT)}};                                 \
      return s;                                                                \
    }                                                                          \
    inline static const ::simpletrace::event_id_t event_id =                   \
        ::simpletrace::trace_registry_t::instance().register_type<name>();     \
  };



