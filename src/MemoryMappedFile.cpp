#include "MemoryMappedFile.h"

MemoryMappedFileHandle MemoryMappedFile::Open(const char* filename, const char* mode) {
  int flags = 0;
  if (mode[0] == 'r' && mode[1] == '\0') {
    flags = O_RDONLY;

  } else if (mode[0] == 'w' && mode[1] == '\0') {
    flags = O_WRONLY;

  } else if (mode[0] == 'r' && mode[1] == 'w' && mode[2] == '\0') {
    flags = O_RDWR;

  } else {
    // Unsupported mode
    return MemoryMappedFileHandle{ -1, 0, nullptr };
  }

  const int fd = open(filename, flags);
  if (fd == -1) {
    return MemoryMappedFileHandle{ -1, 0, nullptr };
  }

  struct stat fileInfo{};
  if (fstat(fd, &fileInfo) == -1) {
    close(fd);
    return MemoryMappedFileHandle{ -1, 0, nullptr };
  }

  void* baseAddress = mmap(nullptr, fileInfo.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (baseAddress == MAP_FAILED) {
    close(fd);
    return MemoryMappedFileHandle{ -1, 0, nullptr };
  }

  return MemoryMappedFileHandle{ fd, static_cast<size_t>(fileInfo.st_size), baseAddress };
}

void MemoryMappedFile::Close(const MemoryMappedFileHandle &handle) {
  if (handle.baseAddress != nullptr) {
    munmap(handle.baseAddress, handle.size);
  }
  if (handle.fd != -1) {
    close(handle.fd);
  }
}