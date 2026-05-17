#include <phnt_windows.h>
#include <phnt.h>
#include <ntpebteb.h>

extern "C" void putchar_(char c)
{
    DWORD written;
    HANDLE consoleHandle = NtCurrentPeb()->ProcessParameters->StandardOutput;

    WriteFile(consoleHandle, &c, 1, &written, nullptr);
}
