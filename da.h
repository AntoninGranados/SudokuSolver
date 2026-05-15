#include <math.h>

#define da_append(da, x) do {                                                      \
    if ((da)->count >= (da)->capacity) {                                           \
        if ((da)->capacity == 0) (da)->capacity = 16;                              \
        else (da)->capacity *= 2;                                                  \
        (da)->items = realloc((da)->items, (da)->capacity * sizeof(*(da)->items)); \
    }                                                                              \
    (da)->items[(da)->count++] = x;                                                \
} while (0)                                                                        \

#define da_resize(da, n) do {                                       \
    if (n > (da)->count) {                                          \
        float pow = ceilf(log2f((float)n));                         \
        (da)->capacity = (size_t)powf(2.0f, pow);                   \
        (da)->items = calloc((da)->capacity, sizeof(*(da)->items)); \
    }                                                               \
    (da)->count = n;                                                \
} while (0)                                                         \
