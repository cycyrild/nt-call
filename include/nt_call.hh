/*
* nt-call
* Minimal Windows x64 syscall dispatcher
* https://github.com/cycyrild/nt-call
*/

#pragma once

#include <cstdint>

namespace nt
{
    extern "C" uint64_t NtSyscallBridge(uint32_t syscallNumber,
                                        uint32_t argumentCount,
                                        uintptr_t syscallRetGadget,
                                        const uint64_t *args);

    constexpr uint32_t SYSCALL_MIN_ARGS = 4;
    constexpr uint32_t SYSCALL_MAX_ARGS = 16;

    template <typename T> struct NtPrototype;

    template <typename Ret, typename... ProtoArgs> struct NtPrototype<Ret (*)(ProtoArgs...)>
    {
        using RetType = Ret;

        auto operator()(uint32_t syscallNumber, uintptr_t syscallRetGadget, ProtoArgs... args) const -> Ret
        {
            constexpr uint32_t ArgCount = sizeof...(ProtoArgs);

            static_assert(ArgCount <= SYSCALL_MAX_ARGS, "Too many arguments for a NT syscall");

            int i = 0;
            constexpr auto StoredArgCount = (ArgCount < SYSCALL_MIN_ARGS) ? SYSCALL_MIN_ARGS : ArgCount;

            uint64_t syscallArgs[StoredArgCount];

            ((syscallArgs[i++] = (uint64_t)args), ...);

            return (Ret)NtSyscallBridge(syscallNumber, ArgCount, syscallRetGadget, syscallArgs);
        }
    };

    template <typename Prototype> inline constexpr NtPrototype<Prototype> NtCall;
}
