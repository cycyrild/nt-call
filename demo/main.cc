#include <phnt_windows.h>
#include <phnt.h>
#include <nt_call.hh>
#include <array>
#include <cstdio>

//
// Syscall numbers used in this example.
//
// Target OS:
// Windows 10 x64 21H2
//
// NtReadFile : 0x0006
// NtClose    : 0x000F
// NtOpenFile : 0x0033
//

//
// Offset to the "syscall; ret" sequence
// inside a standard ntdll syscall stub.
//
// Stub layout:
//
// +00 mov r10, rcx
// +03 mov eax, <syscall_number>
// +08 test byte ptr [SharedUserData], 1
// +10 jne ...
// +12 syscall
// +14 ret
//
constexpr uintptr_t SyscallStubOffset = 0x12;

extern "C" int start()
{
    HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");

    FARPROC pNtOpenFile = GetProcAddress(ntdll, "NtOpenFile");
    FARPROC pNtReadFile = GetProcAddress(ntdll, "NtReadFile");
    FARPROC pNtClose = GetProcAddress(ntdll, "NtClose");

    NTSTATUS status;

    std::array<std::uint8_t, 250> buffer;

    HANDLE fileHandle;
    UNICODE_STRING filePath;
    OBJECT_ATTRIBUTES objectAttributes;
    IO_STATUS_BLOCK ioStatus;
    RtlInitUnicodeString(&filePath, L"\\??\\C:\\Windows\\notepad.exe");

    InitializeObjectAttributes(&objectAttributes, &filePath, OBJ_CASE_INSENSITIVE, nullptr, nullptr);

    status = nt::NtCall<decltype(&NtOpenFile)>(0x0033,
                                               (uintptr_t)pNtOpenFile + SyscallStubOffset,

                                               &fileHandle,
                                               FILE_GENERIC_READ | SYNCHRONIZE,
                                               &objectAttributes,
                                               &ioStatus,
                                               FILE_SHARE_READ,
                                               FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT);

    printf("NtOpenFile -> status: 0x%08lX | handle: %p\n", status, fileHandle);

    status = nt::NtCall<decltype(&NtReadFile)>(0x0006,
                                               (uintptr_t)pNtReadFile + SyscallStubOffset,

                                               fileHandle,
                                               nullptr,
                                               nullptr,
                                               nullptr,
                                               &ioStatus,
                                               buffer.data(),
                                               static_cast<ULONG>(buffer.size()),
                                               nullptr,
                                               nullptr);

    printf("NtReadFile -> status: 0x%08lX\n", status);

    for (const auto byte : buffer)
        printf("%02X ", byte);

    printf("\n");

    status = nt::NtCall<decltype(&NtClose)>(0x000F, (uintptr_t)pNtClose + SyscallStubOffset, fileHandle);

    printf("NtClose -> status: 0x%08lX\n", status);
    return 0;
}
