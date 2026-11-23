#ifndef LC_DA_H
#define LC_DA_H

#ifdef LC_DA_STRIP_PREFIX
#define da_realloc lc_da_realloc
#define da_type    lc_da_type
#define da_append  lc_da_append
#endif

#ifndef lc_da_realloc
#define lc_da_realloc realloc
#include <stddef.h>
extern void *realloc(void *p, size_t size);
#endif

#ifndef lc_da_type
#include <stddef.h>

#define lc_da_type(type, ...) struct __VA_ARGS__ {                                                  \
	size_t count;                                                                               \
	size_t capacity;                                                                            \
	type *data;                                                                                 \
}


#define lc_da_append(list, item) do {                                                               \
	if (0 == (list)->capacity) {                                                                \
		(list)->capacity = 16;                                                              \
		(list)->data     = lc_da_realloc((list)->data, sizeof (item) * (list)->capacity);   \
	}                                                                                           \
	if ((list)->count >= (list)->capacity) {                                                    \
		while ((list)->count >= (list)->capacity) {                                         \
			(list)->capacity *= 2;                                                      \
		}                                                                                   \
		(list)->data = lc_da_realloc((list)->data, sizeof (item) * (list)->capacity);       \
	}                                                                                           \
	(list)->data[(list)->count++] = (item);                                                     \
} while (false)

#endif

#endif
