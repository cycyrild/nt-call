#include <algorithm>
#include <limits>
#include <ntdll.h>

extern "C" NTSTATUS NTAPI RtlInitUnicodeString(PUNICODE_STRING destination, PCWSTR source)
{
    if (destination == nullptr)
        return STATUS_INVALID_PARAMETER;

    if (source == nullptr)
    {
        destination->Length = 0;
        destination->MaximumLength = 0;
        destination->Buffer = nullptr;

        return STATUS_SUCCESS;
    }

    constexpr auto maxLength = std::numeric_limits<USHORT>::max() - sizeof(WCHAR);

    auto length = std::min(wcslen(source) * sizeof(WCHAR), maxLength);

    destination->Length = static_cast<USHORT>(length);
    destination->MaximumLength = static_cast<USHORT>(length + sizeof(WCHAR));
    destination->Buffer = const_cast<PWSTR>(source);

    return STATUS_SUCCESS;
}
