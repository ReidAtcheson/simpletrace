#pragma once
#include "trace_registry.h"

#include <span>

namespace simpletrace {

class trace_writer_t {
public:
  virtual void write(const event_id_t event,
                     std::span<const std::byte> data) = 0;
  virtual void flush() = 0;
  /*Can write a concept for T.*/
  template<typename T>
  void write_type(const T& x){
    write(T::event_id,std::span<const std::byte>(reinterpret_cast<const std::byte*>(&x,sizeof(T))));
  }
};

} // namespace simpletrace
