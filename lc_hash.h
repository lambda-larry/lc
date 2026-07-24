#ifndef LC_HASH_H
#define LC_HASH_H

#ifndef LC_HASH_API
#define LC_HASH_API static inline
#endif

#include <stdint.h>
#include <stddef.h>

#define FNV1_64_PRIME  INT64_C(0xCBF29CE484222325)
#define FNV1_64_OFFSET INT64_C(0x00000100000001B3)

#define FNV1_32_PRIME  INT32_C(0x01000193)
#define FNV1_32_OFFSET INT32_C(0X811C9DC5)

LC_HASH_API uint64_t
lc_hash_fnv1_64(size_t len, const char s[len])
{
	uint64_t hash = FNV1_64_OFFSET;
	for (size_t i = 0; i < len; i++) {
		hash *= FNV1_64_PRIME;
		hash ^= (uint8_t)s[i];
	}
	return hash;
}

LC_HASH_API uint32_t
lc_hash_fnv1_32(size_t len, const char s[len])
{
	uint32_t hash = FNV1_32_OFFSET;
	for (size_t i = 0; i < len; i++) {
		hash *= FNV1_32_PRIME;
		hash ^= (uint8_t)s[i];
	}
	return hash;
}

LC_HASH_API uint64_t
lc_hash_fnv1a_64(size_t len, const char s[len])
{
	uint64_t hash = FNV1_64_OFFSET;
	for (size_t i = 0; i < len; i++) {
		hash ^= (uint8_t)s[i];
		hash *= FNV1_64_PRIME;
	}
	return hash;
}

LC_HASH_API uint32_t
lc_hash_fnv1a_32(size_t len, const char s[len])
{
	uint32_t hash = FNV1_32_OFFSET;
	for (size_t i = 0; i < len; i++) {
		hash ^= (uint8_t)s[i];
		hash *= FNV1_32_PRIME;
	}
	return hash;
}


#endif
