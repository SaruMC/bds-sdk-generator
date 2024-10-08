#include "MemoryMappedFile.h"
#include <stdexcept>

MemoryMappedFileHandle MemoryMappedFile::Open(const char* filename, const char* mode) {
  const int fd = open(filename, (strcmp(mode, "r") == 0) ? O_RDONLY : O_RDWR);
  if (fd == -1) {
    throw std::runtime_error("Failed to open the file.");
  }

  struct stat fileInfo{};
  if (fstat(fd, &fileInfo) == -1) {
    close(fd);
    throw std::runtime_error("Failed to get file status.");
  }

  void* baseAddress = mmap(nullptr, fileInfo.st_size, (strcmp(mode, "r") == 0) ? PROT_READ : PROT_WRITE, MAP_SHARED, fd, 0);
  if (baseAddress == MAP_FAILED) {
    close(fd);
    throw std::runtime_error("Failed to map the file.");
  }

  return { fd, static_cast<size_t>(fileInfo.st_size), baseAddress };
}

void MemoryMappedFile::Close(const MemoryMappedFileHandle &handle) {
  if (munmap(handle.baseAddress, handle.size) == -1) {
    throw std::runtime_error("Failed to unmap the file.");
  }
  close(handle.fd);
}