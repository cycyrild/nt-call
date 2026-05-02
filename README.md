# nt-call

Minimal NT syscall dispatcher for Windows x64.

`nt-call` provides a generic interface for invoking NT system calls using:

- runtime syscall numbers
- external `syscall; ret` stubs
- a single syscall bridge

## Use Cases

- Direct NT syscall dispatching
- Runtime-oriented alternative to SysWhispers-like stub generation

## Example

```cpp
HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");

FARPROC pNtClose =
    GetProcAddress(ntdll, "NtClose");

NTSTATUS status =
    nt::NtCall<decltype(&NtClose)>(
        0x000F,                     // Syscall number
        (uintptr_t)pNtClose + 0x12, // "syscall; ret" stub
        handle                      // NtClose(HANDLE)
    );
```
