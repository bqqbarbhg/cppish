#pragma once

#include <boa_core_cpp.h>

struct ident_str {
	uint32_t Length;
	char Data[1];
};

struct ident {
	const ident_str *String;
};

ident InternIdentHash(const char *str, uint32_t length, uint32_t hash);
ident InternIdent(const char *str, uint32_t length);
inline ident InternIdent(const char *str) {
	return InternIdent(str, strlen(str));
}

using ident_set = boa::u32_set<ident>;

template <typename T>
using ident_map = boa::u32_map<ident, T>;
