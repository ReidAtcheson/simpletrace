#pragma once
#include "trace_registry.h"

#include <span>


namespace simpletrace{

class trace_writer_t{
  public:
    virtual void write(const event_id_t event,std::span<const std::byte> data) = 0;
    virtual void flush() = 0;
};

}
