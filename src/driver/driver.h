#pragma once
#include <windows.h>
#include <TlHelp32.h>
#include <vector>
#include <string>
#include <memory>

extern "C" intptr_t readvm(
    HANDLE ProcessHandle,
    PVOID BaseAddress,
    PVOID Buffer,
    ULONG NumberOfBytesToRead,
    PULONG NumberOfBytesRead
);

extern "C" intptr_t writevm(
    HANDLE ProcessHandle,
    PVOID BaseAddress,
    PVOID Buffer,
    ULONG NumberOfBytesToWrite,
    PULONG NumberOfBytesWritten
);

union rbxdata {
    char Inline[16];
    std::uint64_t Pointer;
};

struct rbxstring {
    rbxdata Data;
    std::uint64_t Length;
    std::uint64_t Capacity;
};

class driver final {
public:
    driver() = default;
    ~driver() = default;

    std::uint32_t process(const std::string& Process_Name);
    std::uint64_t module(const std::string& Module_Name);
    bool attach(const std::string& Process_Name);

    std::string readstring(std::uint64_t Address);
    void writestring(std::uint64_t Address, const std::string& Value);

    template <typename T>
    T read(std::uint64_t Address);

    template <typename T>
    void write(std::uint64_t Address, T Value);

    std::uint32_t processid();
    std::uint64_t modulebase();
    HANDLE processhandle();

private:
    std::uint32_t pid;
    std::uint64_t base;
    HANDLE handle;
};

template <typename T>
T driver::read(std::uint64_t Address) {
    T Buffer{};
    readvm(handle, reinterpret_cast<void*>(Address), &Buffer, sizeof(T), nullptr);
    return Buffer;
}

template <typename T>
void driver::write(std::uint64_t Address, T Value) {
    writevm(handle, reinterpret_cast<void*>(Address), &Value, sizeof(T), nullptr);
}

inline std::unique_ptr<driver> drive = std::make_unique<driver>();
