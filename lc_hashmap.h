#ifndef LC_HM_H
#define LC_HM_H

#ifndef LC_HM_INIT_SIZE
#define LC_HM_INIT_SIZE 16
#endif

#ifndef LC_HM_LOAD_FACTOR_HIGH
#define LC_HM_LOAD_FACTOR_HIGH 70
#endif

#ifndef LC_HM_LOAD_FACTOR_LOW
#define LC_HM_LOAD_FACTOR_LOW 50
#endif

#ifndef LC_HM_API
#define LC_HM_API extern
#endif

#include <stddef.h>
#include <stdbool.h>

struct lc_hash_entry {
	size_t keylen;
	const char *key;
	const void *val;
};

struct lc_hashmap {

	size_t count;
	size_t capacity;
	struct lc_hash_entry *buckets;

#ifdef LC_HM_CUSTOM_ALLOC

	void *alloc_ctx;
	void *(*realloc)(void *ctx, void *p, size_t size);

#endif

};

LC_HM_API bool lc_hashmap_get(const struct lc_hashmap *map, size_t n, const char key[restrict n], void *restrict out);
LC_HM_API bool lc_hashmap_put(struct lc_hashmap *map, size_t n, const char key[restrict n], const void *val, void *out);
LC_HM_API bool lc_hashmap_del(struct lc_hashmap *map, size_t n, const char key[restrict n], void *restrict out);
LC_HM_API bool lc_hashmap_iter(const struct lc_hashmap *map, size_t *it);

/**
 * @breif Iterate the hashmap
 *
 * The function was designed to be used in the condition in for loop.
 *
 * @param map[in]  Hashmap to iterate
 * @param it[out]  Pointer to index as iterator
 *
 * @return false, if iteration is done
 */
LC_HM_API bool lc_hashmap_iter(const struct lc_hashmap *map, size_t *it);



#if 0
LC_HM_API bool lc_hashmap_iter(const struct lc_hashmap *map, size_t *it);

// Example iteration
struct lc_hashmap map = {0};

/// ...

for (size_t i = 0; lc_hashmap_iter(&map, &i); i++) {
	const void *key = map.buckets[i].key;
	const void *val = map.buckets[i].val;
}

#endif


#ifdef LC_HAVE_SV

static inline bool
lc_hashmap_get_sv(const struct lc_hashmap *map, struct lc_sv key, void *restrict out)
{
	return lc_hashmap_get(map, key.length, key.s, out);
}

static inline bool
lc_hashmap_put_sv(const struct lc_hashmap *map, struct lc_sv key, const void *val, void *out)
{
	return lc_hashmap_put(map, key.length, key.s, val, out);
}

static inline bool
lc_hashmap_del_sv(const struct lc_hashmap *map, struct lc_sv key, void *restrict out)
{
	return lc_hashmap_del(map, key.length, key.s, out);
}

#endif



#ifdef LC_IMPLEMENTATION


#ifdef LC_HM_CUSTOM_ALLOC

static void *
_lc_hm_realloc(void *_, void *p, size_t size)
{
	extern void *memset(void *, int, size_t);
	extern void *realloc(void *, size_t);
	extern void free(void *);

	if (size)
		return memset(realloc(p, size), 0, size);

	free(p);
	return 0;
}

#define lc_hm_calloc(n, size) map->realloc(map->alloc_ctx, 0, (n) * (size))
#define lc_hm_free(p)         map->realloc(map->alloc_ctx, p, 0)

#else

extern void *calloc(size_t n, size_t size);
extern void free(void *);

#define lc_hm_calloc(n, size) calloc(n, size)
#define lc_hm_free(p)         free(p)

#endif

#include <stdint.h>

#ifdef LC_HM_FNV1A_64BIT
typedef uint64_t _lc_hm_hash;

#define FNV1_PRIME  0xCBF29CE484222325
#define FNV1_OFFSET 0x00000100000001B3
#else
typedef uint32_t _lc_hm_hash;

#define FNV1_PRIME  0x01000193
#define FNV1_OFFSET 0X811C9DC5
#endif

static inline _lc_hm_hash
_lc_hm_fnv1a_hash(size_t len, const char s[len])
{
	_lc_hm_hash hash = FNV1_OFFSET;
	for (int i = 0; i < len; i++) {
		hash ^= (uint8_t)s[i];
		hash *= FNV1_PRIME;
	}
	return hash;
}

static inline bool
key_entry_cmp(
	const struct lc_hash_entry *entry,
	size_t n,
	const char key[restrict n]
)
{
	extern int memcmp(const void *, const void *, size_t);
	return entry->keylen == n
	    && 0 == memcmp(entry->key, key, n);
}

static size_t
find_bucket(const struct lc_hashmap *map, size_t n, const char key[restrict n])
{
	const _lc_hm_hash hash = _lc_hm_fnv1a_hash(n, key);

	size_t i = hash % map->capacity;

	while (map->buckets[i].key && !key_entry_cmp(&map->buckets[i], n, key))
		i++, i %= map->capacity;

	return i;
}

static void
rehash(struct lc_hashmap *map)
{
	size_t keys = map->count;

	size_t capacity = map->capacity;
	while ((keys * 100) / capacity >= LC_HM_LOAD_FACTOR_LOW)
		capacity *= 2;

	struct lc_hashmap new_map = {
		.buckets  = lc_hm_calloc(capacity, sizeof new_map.buckets[0]),
		.capacity = capacity,
	};


	for (size_t i = 0; i < map->capacity; i++) {
		struct lc_hash_entry *entry = &map->buckets[i];

		if (!entry->key)
			continue;

		lc_hashmap_put(&new_map, entry->keylen, entry->key, entry->val, &(void *){0});
	}

#ifdef assert
	assert(new_map.count == keys);
#endif
	lc_hm_free(map->buckets);
	*map = new_map;
}

LC_HM_API bool
lc_hashmap_get(const struct lc_hashmap *map, size_t n, const char key[restrict n], void *restrict out)
{
	size_t i = find_bucket(map, n, key);
	const void **o = out;

	if (map->buckets[i].key)
		return (*o = map->buckets[i].val), true;

	return (*o = 0);
}

LC_HM_API bool
lc_hashmap_put(struct lc_hashmap *map, size_t n, const char key[restrict n], const void *val, void *out)
{

#ifdef LC_HM_CUSTOM_ALLOC
	if (!map->realloc)
		map->realloc = _lc_hm_realloc;

#endif

	if (!map->buckets) {
		map->buckets  = lc_hm_calloc(LC_HM_INIT_SIZE, sizeof map->buckets[0]);
		map->capacity = LC_HM_INIT_SIZE;
	}

	size_t i = find_bucket(map, n, key);
	const void **o = out;

	if (map->buckets[i].key) {
		*o = map->buckets[i].val;
		map->buckets[i].val = val;
		return true;
	}

	if ((map->count * 100) / map->capacity >= LC_HM_LOAD_FACTOR_HIGH) {
		rehash(map);
		i = find_bucket(map, n, key);
	}

	map->count++;
	map->buckets[i].key    = key;
	map->buckets[i].keylen = n;
	map->buckets[i].val    = val;


	return false;
}

LC_HM_API bool
lc_hashmap_del(struct lc_hashmap *map, size_t n, const char key[restrict n], void *restrict out)
{
	size_t i = find_bucket(map, n, key);

	if (!map->buckets[i].key)
		return false;

	const void **o = out;
	*o = map->buckets[i].val;

	map->buckets[i] = (struct lc_hash_entry){0};
	map->count--;

	size_t j = i;
	for (;;) {
		j += 1;
		j %= map->capacity;

		if (!map->buckets[j].key)
			break;

		const size_t k = _lc_hm_fnv1a_hash(map->buckets[j].keylen, map->buckets[j].key) % map->capacity;

		if (i <= j) {
			if (i < k && k <= j)
				continue;
		} else {
			if (k <= j || i < k)
				continue;
		}

		map->buckets[i] = map->buckets[j];
		map->buckets[j] = (struct lc_hash_entry){0};
		i = j;
	}

	return true;
}

LC_HM_API bool
lc_hashmap_iter(const struct lc_hashmap *map, size_t *it)
{
	if (map->capacity <= *it)
		return false;

	for (size_t i = *it; i < map->capacity; i++) {
		if (!map->buckets[i].key)
			continue;

		*it = i;
		return true;
	}

	*it = map->capacity;
	return false;
}

#endif

// vim: set ft=c:
#endif
