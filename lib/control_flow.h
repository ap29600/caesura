#ifndef FZR_CONTROL
#define FZR_CONTROL

#define with(begin, end)                                                       \
  for (begin, *__cond##__LINE__ = (void *)1; \
      __cond##__LINE__;                 \
       (__cond##__LINE__ = 0, end))

#define defer(...)                                                             \
  for (bool __cond##__LINE__ = true; __cond##__LINE__;                         \
       __cond##__LINE__ = false, __VA_ARGS__)

#endif
