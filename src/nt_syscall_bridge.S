/*
* nt-call
* Windows x64 syscall bridge
* https://github.com/cycyrild/nt-call
*/

.intel_syntax noprefix
.text

.equ SHADOW_SPACE, 32

.global NtSyscallBridge
NtSyscallBridge:
        push    rbp
        mov     rbp, rsp

        mov     r10d, ecx
        mov     r11,  r8

        mov     ecx, edx

        cmp     ecx, 4
        ja      .has_extra_args

        xor     ecx, ecx
        jmp     .extra_count_ok

.has_extra_args:
        sub     ecx, 4

.extra_count_ok:
        lea     rax, [rcx*8 + SHADOW_SPACE]

        mov     edx, ecx
        and     edx, 1
        lea     rax, [rax + rdx*8]

        sub     rsp, rax

        test    ecx, ecx
        jz      .no_extra_args

        lea     r8,  [r9 + 4*8]
        lea     rax, [rsp + SHADOW_SPACE]

.copy_loop:
        mov     rdx, [r8]
        mov     [rax], rdx

        add     r8, 8
        add     rax, 8

        dec     ecx
        jnz     .copy_loop

.no_extra_args:
        mov     eax, r10d

        mov     rcx, [r9 + 0*8]
        mov     rdx, [r9 + 1*8]
        mov     r8,  [r9 + 2*8]
        mov     r9,  [r9 + 3*8]

        mov     r10, rcx

        call    r11

        mov     rsp, rbp
        pop     rbp
        ret
