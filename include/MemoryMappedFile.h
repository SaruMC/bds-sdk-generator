#pragma once

#ifndef MEMORYMAPPEDFILE_H
#define MEMORYMAPPEDFILE_H

#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

struct MemoryMappedFileHandle {
    int fd;
    size_t size;
    void* baseAddress;
};

class MemoryMappedFile {
  public:
    static MemoryMappedFileHandle Open(const char* filename, const char* mode);
    static void Close(MemoryMappedFileHandle handle);
};

#endif //MEMORYMAPPEDFILE_H