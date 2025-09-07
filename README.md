# simpletrace
An incredibly simple c++ tracing library. 

If you found this you probably want something like [binlog](https://github.com/erenon/binlog) and not
this project.

This is more of a simple library to help with my own tracing needs within personal projects. My goals were

1. extremely lightweight binary logging that could multipurpose as tracing 
2. a very simple schema that could allow logs to be extracted to sqlite, tabular format, etc
3. tiny overhead if runtime disabled, and compile-time-disable for zero overhead.
