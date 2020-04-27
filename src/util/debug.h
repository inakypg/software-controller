
#include <stdio.h>

#include <HardwareSerial.h>

#ifndef DEBUG_MODULE
#error "Cannot include debug.h without defining DEBUG_MODULE"
#endif

#ifndef DEBUG_BUFFER_SIZE
#define DEBUG_BUFFER_SIZE 128
#endif

#ifndef DEBUG_SERIAL_PORT
#define DEBUG_SERIAL_PORT Serial
#endif

// Initial setup of debug; only to be used once
#define DEBUG_BEGIN DEBUG_SERIAL_PORT.begin(115200)

#ifdef DEBUG
#ifdef DEBUG_PLOT

#define PLOT(var1,var2,var3)			\
do {                                                \
    DEBUG_SERIAL_PORT.print(var1);                       \
    DEBUG_SERIAL_PORT.print(" ");                       \
    DEBUG_SERIAL_PORT.print(var2);                       \
    DEBUG_SERIAL_PORT.print(" ");                       \
    DEBUG_SERIAL_PORT.println(var3);                       \
  } while (0);

#define DEBUG_PRINT(...)
#define DEBUG_PRINT_EVERY(...)

#else
// Use DEBUG_PRINT to add print messages like printf
#define DEBUG_PRINT(...)                              \
  do {                                                \
    char _buf[DEBUG_BUFFER_SIZE];                     \
    snprintf(_buf, DEBUG_BUFFER_SIZE, __VA_ARGS__);   \
    DEBUG_SERIAL_PORT.print("[" DEBUG_MODULE "] ");        \
    DEBUG_SERIAL_PORT.println(_buf);                       \
  } while (0);

// Use DEBUG_PRINT_EVERY to add print messages line printf, but it will only
// print the message every i times the line is reached (note: it costs 2 bytes
// to remember this per line)
#define DEBUG_PRINT_EVERY(i, ...)                     \
  do {                                                \
    static unsigned int _count = 0;                   \
    if ((++_count) / i) {                             \
      DEBUG_PRINT(__VA_ARGS__);                       \
      _count = 0;                                     \
    }                                                 \
  } while (0);

#define PLOT(...)

#endif

#else
#define PLOT(...)
#define DEBUG_PRINT(...)
#define DEBUG_PRINT_EVERY(...)
#endif
