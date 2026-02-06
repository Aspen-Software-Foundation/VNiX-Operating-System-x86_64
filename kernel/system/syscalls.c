/*
    Copyright (C) 2026 Aspen Software Foundation

    Module: syscalls.c
    Description: The syscalls module for the VNiX Operating System.
    Author: Yazin Tantawi

    All components of the VNiX Operating System, except where otherwise noted, 
    are copyright of the Aspen Software Foundation (and the corresponding author(s)) and licensed under GPLv2 or later.
    For more information on the Gnu Public License Version 2, please refer to the LICENSE file
    or to the link provided here: https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html

 * THIS OPERATING SYSTEM IS PROVIDED "AS IS" AND "AS AVAILABLE" UNDER 
 * THE GNU GENERAL PUBLIC LICENSE VERSION 2, WITHOUT
 * WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, TITLE, AND NON-INFRINGEMENT.
 * 
 * TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, IN NO EVENT SHALL
 * THE AUTHORS, COPYRIGHT HOLDERS, OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE), ARISING IN ANY WAY OUT OF THE USE OF THIS OPERATING SYSTEM,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE OPERATING SYSTEM IS
 * WITH YOU. SHOULD THE OPERATING SYSTEM PROVE DEFECTIVE, YOU ASSUME THE COST OF
 * ALL NECESSARY SERVICING, REPAIR, OR CORRECTION.
 *
 * YOU SHOULD HAVE RECEIVED A COPY OF THE GNU GENERAL PUBLIC LICENSE
 * ALONG WITH THIS OPERATING SYSTEM; IF NOT, WRITE TO THE FREE SOFTWARE
 * FOUNDATION, INC., 51 FRANKLIN STREET, FIFTH FLOOR, BOSTON,
 * MA 02110-1301, USA.
*/

#include <stdint.h>
#include "tools/includes/log-info.h"
#include "tools/includes/serial.h"
#include "includes/syscalls.h"
#include "kernel/terminal/src/flanterm_backends/fb.h"
#include "kernel/terminal/src/flanterm.h"
#include "kernel/shell/includes/keyboard.h"

extern struct flanterm_context *global_flanterm;

typedef uint64_t (*syscall_fn_t)(
    uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t
);

static syscall_fn_t syscall_table[SYS_MAX] = {0};

uint64_t sys_write(uint64_t fd, uint64_t buf, uint64_t len,
                   uint64_t a4, uint64_t a5, uint64_t a6) {
    (void)a4; (void)a5; (void)a6;

    if (!buf || (fd != 1 && fd != 2) || !global_flanterm)
        return (uint64_t)-1;

    flanterm_write(global_flanterm, (const char *)buf, (size_t)len);
    return len;
}


uint64_t sys_exit(uint64_t code, uint64_t a1, uint64_t a2,
                  uint64_t a3, uint64_t a4, uint64_t a5) {
    (void)code; (void)a1; (void)a2; (void)a3; (void)a4; (void)a5;
    for (;;)
        __asm__ volatile("hlt");
    return 0;
}

uint64_t sys_read(uint64_t fd, uint64_t buf_ptr, uint64_t count,
                  uint64_t a4, uint64_t a5, uint64_t a6) {
    (void)a4; (void)a5; (void)a6;

    if (!buf_ptr) return (uint64_t)-1;
    char *buf = (char *)buf_ptr;

    if (fd == 0) { // stdin
        for (uint64_t i = 0; i < count; i++) {
            char c = getc();   // get one char from keyboard
            buf[i] = c;

            if (c == '\n') {
                return i + 1; // include newline in count
            }
        }
        return count;
    }

    return (uint64_t)-1; // unsupported fd
}

void init_syscall_table(void) {
    syscall_table[SYS_write] = sys_write;
    syscall_table[SYS_exit]  = sys_exit;
}

uint64_t handle_syscall(uint64_t num,
                        uint64_t a1, uint64_t a2, uint64_t a3,
                        uint64_t a4, uint64_t a5) {
    if (num >= SYS_MAX || !syscall_table[num])
        return (uint64_t)-1;

    return syscall_table[num](a1, a2, a3, a4, a5, 0);
}

#define MSR_STAR    0xC0000081
#define MSR_LSTAR   0xC0000082
#define MSR_SFMASK  0xC0000084

static inline void wrmsr(uint32_t msr, uint64_t value) {
    uint32_t low  = (uint32_t)(value & 0xFFFFFFFF);
    uint32_t high = (uint32_t)(value >> 32);

    __asm__ volatile (
        "wrmsr"
        :
        : "c"(msr), "a"(low), "d"(high)
        : "memory"
    );
}

extern void syscall_handler(void);

void syscall_init(void) {
    uint64_t star = ((uint64_t)0x08 << 32) |   // kernel CS
                    ((uint64_t)0x18 << 48);    // user CS base

    wrmsr(MSR_STAR, star);
    wrmsr(MSR_LSTAR, (uint64_t)syscall_handler);
    wrmsr(MSR_SFMASK, (1ULL << 9));

    init_syscall_table();

    LOG_INFO("Syscalls successfully initialized\n");
}

static inline void _exit(int code) {
    __asm__ volatile(
        "mov %[num], %%rax\n"
        "mov %[val], %%rdi\n"
        "syscall\n"
        :
        : [num]"i"(SYS_exit), [val]"r"((uint64_t)code)
        : "rax", "rdi"
    );
    __builtin_unreachable();
}
