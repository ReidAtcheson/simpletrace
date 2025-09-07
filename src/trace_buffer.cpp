#include "trace_buffer.h"

#include <limits>

namespace simpletrace{

trace_buffer_t::trace_buffer_t(const size_t nbytes) : data_(nbytes), mr_(&data_[0],nbytes,std::pmr::null_memory_resource()), pa_(&mr_),drain_mr_(&data_[0],nbytes,std::pmr::null_memory_resource()),drain_pa_(&drain_mr_) {}

/*This is the most generic possible behavior with heterogeneous event types.
 *
 * But one could imagine a simple optimization where in the construction of trace_buffer_t we check
 * that all event_types currently registered have the same size and alignment, and a lot of
 * checking done here becomes unnecessary in that case.
 *
 * Perhaps more generally we could require all trace events have the same high-ish alignment
 * (e.g. 32 bytes) and size<=max_size, and stick them in constant slots. Either way it is
 * compatible with the below generic implementation and should rather be seen as an optimization
 * or special case.
 *
 * */
bool trace_buffer_t::buffer(const event_id_t event,std::span<const std::byte> data) noexcept{
  assert(data.size()>0);
  assert(nevents_ < std::numeric_limits<event_id_t>::max() );
  try{
    size_t align = trace_registry_t::instance().schema(event).align;
    size_t size = trace_registry_t::instance().schema(event).size;
    assert(size == data.size());
    void* event_type_p = pa_.allocate_bytes(sizeof(event_id_t),alignof(event_id_t));
    *reinterpret_cast<event_id_t*>(event_type_p) = event;
    void* event_p = pa_.allocate_bytes(size,align);
    std::memcpy(event_p,&data[0],data.size());
  } catch(const std::bad_alloc&){
    /*We ran out of space. need to drain events and reset.*/
    return false;
  }
  nevents_+=1;
  return true;
}

trace_buffer_t::drained_event_t trace_buffer_t::drain_single() {
  /*Note I don't catch the possible bad_alloc here. The way this class is set up it shouldn't possibly throw unless
   * something very bad has happened.*/
  if(nevents_==0) return {.event = std::numeric_limits<event_id_t>::max(), .data = {}, .events_remaining = 0};
  event_id_t event = *reinterpret_cast<event_id_t*>(drain_pa_.allocate_bytes(sizeof(event_id_t),alignof(event_id_t)));
  size_t align = trace_registry_t::instance().schema(event).align;
  size_t size = trace_registry_t::instance().schema(event).size;
  std::byte* d = reinterpret_cast<std::byte*>(drain_pa_.allocate_bytes(size,align));
  nevents_-=1;
  return {.event = event, .data = std::span<const std::byte>(d,size), .events_remaining = nevents_};
}

void trace_buffer_t::reset(){
  nevents_ = 0;
  mr_.release();
  drain_mr_.release();
}




}
