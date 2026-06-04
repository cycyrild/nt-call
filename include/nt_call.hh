/*
* nt-call
* Minimal Windows x64 syscall dispatcher
* https://github.com/cycyrild/nt-call
*/

#pragma once

#include <cstdint>

namespace nt
{
    extern "C" uint64_t NtSyscallBridge(uint32_t syscallNumber, uintptr_t syscallRetGadget, ...);

    constexpr uint32_t SYSCALL_MAX_ARGS = 16;

    template <typename T> struct NtPrototype;

    template <typename Ret, typename... ProtoArgs> struct NtPrototype<Ret (*)(ProtoArgs...)>
    {
        auto operator()(uint32_t syscallNumber, uintptr_t syscallRetGadget, ProtoArgs... args) const -> Ret
        {
            constexpr uint32_t ArgCount = sizeof...(ProtoArgs);

            static_assert(ArgCount <= SYSCALL_MAX_ARGS, "Too many arguments for a NT syscall");

            return (Ret)(NtSyscallBridge(syscallNumber, syscallRetGadget, args...));
        }
    };

    template <typename Prototype> inline constexpr NtPrototype<Prototype> NtCall;
}
