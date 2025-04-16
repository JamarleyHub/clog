# CLOG - A simple C logging library

CLOG is a simple C logging library that provides a convenient way to log messages with different severity levels.

It is fairly lightweight, mostly multi-threading safe and easy to use, making it suitable for both small and medium
sized projects.
It is by no means an enterprise-grade solution, but it is a good starting point for when you need a small but functional
logger.

It focuses on the most basic aspects of logging, so it is by no means as feature-complete as other C implementations.
*BUT!* It narrowly beats printf debugging ¯\\\_(ツ)_/¯.

It supports logging to a file and allows you to customize varying severity levels.

## Build

To build the library, you can use the provided Makefile. This requires CMake.

To install, simply clone the repo run the following command in the cloned directory:

```bash
cmake -B cmake-build && sudo cmake --install cmake-build
```

This will install the library as a shared object into your include directories.

## Usage

You need to include clog in your CMakeLists.txt file:

```cmake
find_package(clog REQUIRED)
target_link_libraries(YourProjectName PRIVATE clog::clog)
```

You can then use it by simply including this header:

```c
#include <clog.h>
```

You can either generate different contexts for the logger by registering it directly.
Simply provide it with a logging level and a directory to write the logs to.

```c
struct logger_ctx* ctx = register_logger( LEVEL_LOG_DEBUG, ".test_dir" );
```

or you can generate a logging context once globally and call it externally:

```c
#include <clog.h>

extern struct logger_ctx* ctx;
```

As a default setting, it saves the last 10 logs.
The naming convention is `log-YYYY-MM-DD_HH-MM-SS.log` and the logs are saved in the directory you provided.

You can change or retrieve the current log level with calls to `set_log_level` and `get_log_level` respectively.

You can unregister the logger when you are done like so:

```c
unregister_logger( &ctx );
```

To log messages you can either manually call the logger function directly (which is messy and not recommended):

```c
logger( struct logger_ctx* ctx, enum CLOG_LOG_LEVEL level, const char* fmt, ... );
```

or use the convenience macros provided:

```c
CLOG_DEBUG( ctx, "This is a debug message" );
CLOG_INFO( ctx, "This is an info message" );
CLOG_WARN( ctx, "This is a warning message" );
CLOG_ERROR( ctx, "This is an error message" );
CLOG_FATAL( ctx, "This is a fatal message" );
```

This will produce a log like this:

``` 
21:42:26.30 - [DEBUG]: This is a debug message
21:42:26.30 - [INFO]: This is an info message
21:42:26.30 - [WARN]: This is a warning message
21:42:26.30 - [ERROR]: This is an error message
21:42:26.30 - [FATAL]: This is a fatal message
```

The logging messages also support variadic arguments, so you can use them like this:

```c
CLOG_DEBUG( ctx, "Message %d - %s", 42, "A teststring" );
```

This will produce a log like this:

```
21:42:27.42 - [DEBUG]: Message 42 - A teststring
```

Refer to the `test-clog.c` file in `example` for a detailed usage example of all the usable features.

## Planned Features

- Add support for dynamically changing the number of saved logs
- Add support for custom log formatting
- Add support for custom log file naming
- Add support for custom log levels
- Add convenience macros for logging structures such as arrays
- Add support for log file size limits