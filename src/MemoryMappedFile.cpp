#include "MemoryMappedFile.h"

#if defined(_WIN32) || defined(_WIN64)
    MemoryMappedFileHandle MemoryMappedFile::Open(const wchar_t* path) {
        HANDLE file = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, nullptr);
        if (file == INVALID_HANDLE_VALUE) {
            return MemoryMappedFileHandle{ INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE, nullptr };
        }

        HANDLE fileMapping = CreateFileMappingW(file, nullptr, PAGE_READONLY, 0, 0, nullptr);
        if (fileMapping == nullptr) {
            CloseHandle(file);
            return MemoryMappedFileHandle{ INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE, nullptr };
        }

        void* baseAddress = MapViewOfFile(fileMapping, FILE_MAP_READ, 0, 0, 0);
        if (baseAddress == nullptr) {
            CloseHandle(fileMapping);
            CloseHandle(file);
            return MemoryMappedFileHandle{ INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE, nullptr };
        }

        return MemoryMappedFileHandle{ file, fileMapping, baseAddress };
    }

    void MemoryMappedFile::Close(MemoryMappedFileHandle& handle) {
        UnmapViewOfFile(handle.baseAddress);
        CloseHandle(handle.fileMapping);
        CloseHandle(handle.file);

        handle.file = nullptr;
        handle.fileMapping = nullptr;
        handle.baseAddress = nullptr;
    }
#else
    MemoryMappedFileHandle MemoryMappedFile::Open(const wchar_t* path) {
        // Convert wchar_t* to char* for POSIX API
        std::string pathStr = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(path);

        // Open the file
        int fd = open(pathStr.c_str(), O_RDONLY);
        if (fd == -1) {
            return MemoryMappedFileHandle{ -1, 0, nullptr };
        }

        // Get the file size
        struct stat sb;
        if (fstat(fd, &sb) == -1) {
            close(fd);
            return MemoryMappedFileHandle{ -1, 0, nullptr };
        }

        size_t size = sb.st_size;

        // Map the file into memory
        void* baseAddress = mmap(nullptr, size, PROT_READ, MAP_SHARED, fd, 0);
        if (baseAddress == MAP_FAILED) {
            close(fd);
            return MemoryMappedFileHandle{ -1, 0, nullptr };
        }

        return MemoryMappedFileHandle{ fd, size, baseAddress };
    }

    void MemoryMappedFile::Close(MemoryMappedFileHandle& handle) {
        munmap(handle.baseAddress, handle.size);
        close(handle.fd);

        handle.fd = -1;
        handle.size = 0;
        handle.baseAddress = nullptr;
    }
#endif