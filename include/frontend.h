#pragma once

#include "base.h"

struct source_file {
	uint32_t Index;
};

struct source_buffer {
	source_file File;
	const char *Data;
	uint32_t Length;
};

source_file CreateFileSource(const char *filename);
source_file CreateDataSource(const char *name, const char *data, uint32_t length);

source_buffer *OpenSourceBuffer(source_file file);
void CloseSourceBuffer(source_buffer *buffer);

struct source_loc {
	source_file SourceFile;
	uint32_t ByteOffset;
};
