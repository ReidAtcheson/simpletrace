# simpletrace
An incredibly simple c++ tracing library. 

If you found this you probably want something like [binlog](https://github.com/erenon/binlog) and not
this project.

This is more of a simple library to help with my own tracing needs within personal projects. My goals were

1. extremely lightweight binary logging that could multipurpose as tracing 
2. a very simple schema that could allow logs to be extracted to sqlite, tabular format, etc
3. tiny overhead if runtime disabled, and compile-time-disable for zero overhead.

The idea being that basic tracing is supported, but that we could add arbitrary data on an ad-hoc
basis that could be immediately understood by consumers (trace sinks & writers).

## Quick start

### Basic scoped trace
```c++
#include "scoped_trace.h"
#include "ndjson_trace_writer.h"

int main() {
  using namespace simpletrace;
  ndjson_trace_writer_t writer{"trace.json", 1 << 20};
  impl::set_tls_writer(&writer);
  {
    SIMPLETRACE_SCOPED_TRACE("doing work");
    // ... your code ...
  }
  writer.flush();
}
```
This writes a beginning and end `scope_trace_event_t` for the labeled block.

### Custom events
Define a struct with the provided macros to describe the fields of the event.
```c++
#define MY_EVENT_FIELDS(X) \
  X(int32_t, answer) \
  X(std::string_view, message)
SIMPLETRACE_EVENT_STRUCT(my_event_t, MY_EVENT_FIELDS);
```
Emitting the event is then just:
```c++
my_event_t e{.answer = 42, .message = "hello"};
writer.write_type(e);
```
The macro registers the type with `trace_registry_t`, so readers know how to decode the bytes.
