#include "base.h"
#include <string.h>

boa_map g_IdentMap;
boa_arena g_IdentArena;

void BaseInitialize()
{
	boa_map_init(&g_IdentMap, sizeof(ident_str*));
}

typedef struct {
	uint32_t Length;
	const char *Data;
} insert_str;

boa_forceinline int IdentCompare(const void *key, const void *entry, void *)
{
	insert_str keyStr = *(const insert_str*)key;
	const ident_str *entryStr = *(const ident_str**)entry;

	uint32_t length = keyStr.Length;
	if (length != entryStr->Length) return 0;
	return !memcmp(keyStr.Data, entryStr->Data, length);
}

ident InternIdentHash(const char *str, uint32_t length, uint32_t hash)
{
	insert_str insert = { length, str };
	auto ires = boa_map_insert(&g_IdentMap, &insert, hash, &IdentCompare, NULL);
	ident_str *&id = boa::check_ptr((ident_str**)ires.entry);
	if (ires.inserted) {
		id = (ident_str*)boa_arena_push_size(&g_IdentArena, length + 1 + sizeof(uint32_t), alignof(uint32_t));
		boa_assert(id != NULL);
		id->Length = length;
		memcpy(id->Data, str, length);
		id->Data[length] = '\0';
	}
	return { id };
}

ident InternIdent(const char *str, uint32_t length)
{
	uint32_t hash = 2166136261u;
	const char *end = str + length;
	for (const char *c = str; c != end; c++) {
		hash = (hash ^ *c) * 16777619;
	}
	return InternIdentHash(str, length, hash);
}
