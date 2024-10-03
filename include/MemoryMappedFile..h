#pragma once

#ifndef MEMORYMAPPEDFILE_H
#define MEMORYMAPPEDFILE_H

#if defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
#else
    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <string.h>
    #include <fcntl.h>
#endif

struct MemoryMappedFileHandle {
  #if defined(_WIN32) || defined(_WIN64)
      HANDLE handle;
      HANDLE mappedFile;
      void* mappedFileBaseAddress;
  #else
      int fd;
      size_t size;
      void* baseAddress;
  #endif
};

class MemoryMappedFile {
  public:
    static MemoryMappedFileHandle open(const char* filename, const char* mode);
    static void close(MemoryMappedFileHandle handle);
};

#endif //MEMORYMAPPEDFILE_H