#include <frontend.h>

struct source_buffer_impl : source_buffer {
	bool IsStatic;
	uint32_t RefCount;
};

struct source_file_impl : source_file {
	ident Filename;
	source_buffer_impl *Buffer;
};

struct data_source_file {
	source_file_impl File;
	source_buffer_impl Buffer;
};

static boa::buf<source_file_impl*> g_Files;
static boa::arena g_Arena;
static ident_map<source_file> g_FileSources;

source_file CreateFileSource(const char *filename)
{
	ident fileId = InternIdent(filename);
	auto ires = g_FileSources.insert_uninitialized(fileId);
	if (ires.inserted) {
		uint32_t index = g_Files.count();
		auto file = boa::make<source_file_impl>();
		file->Filename = fileId;
		file->Buffer = NULL;
		g_Files.push(file);

		ires.entry->key = fileId;
		ires.entry->val.Index = index;
	}

	return ires.entry->val;
}

source_file CreateDataSource(const char *name, const char *data, uint32_t length)
{
	uint32_t index = g_Files.count();
	auto dataFile = boa::make<data_source_file>();
	dataFile->File.Filename = InternIdent(name);
	dataFile->File.Buffer = &dataFile->Buffer;
	dataFile->Buffer.Data = (char*)boa::alloc(length + 2);
	dataFile->Buffer.Length = length;
	dataFile->Buffer.IsStatic = true;
	dataFile->Buffer.File.Index = index;
	dataFile->Buffer.RefCount = 1;
	memcpy(dataFile->Buffer.Data, data, length);
	dataFile->Buffer.Data[length] = '\0';
	dataFile->Buffer.Data[length + 1] = '\0';
	return { index };
}

source_buffer *OpenSourceBuffer(source_file file)
{
	source_file_impl *impl = g_Files[file.Index];
	if (impl->Buffer) {
		if (!impl->Buffer->IsStatic)
			impl->Buffer->RefCount++;
		return impl->Buffer;
	} else {
		auto buffer = boa::make<source_buffer_impl>();
		buffer->IsStatic = false;
		buffer->RefCount = 1;
		impl->Buffer = buffer;
		return buffer;
	}
}

void CloseSourceBuffer(source_buffer *buffer)
{
	auto buffer_impl = (source_buffer_impl*)buffer;
	if (buffer_impl->IsStatic || --buffer_impl->RefCount > 0) return;

	source_file_impl *file_impl = g_Files[buffer->File.Index];
	file_impl->Buffer = NULL;

	boa::free(buffer_impl->Data);
	boa::free(buffer_impl);
}
