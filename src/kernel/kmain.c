#include <core/isr.h>
#include <core/timer.h>
#include <core/debug.h>
#include <mmu/mmap.h>
#include <drivers/screen.h>
#include <drivers/serial.h>
#include <drivers/keyboard.h>
#include <stdio.h>
#include <stdint.h>
#include "kmain.h"

#define CHECK_FLAG(flags, bit) ((flags) & (1 << (bit)))

void kmain(unsigned long magic, unsigned long addr)
{
    screen_init();
    screen_clear();

    if (multiboot_is_valid(magic, addr) != 0) {
        printf("System halted!");
        return;
    }

    printf("%s\n", KERNEL_ASCII);
    printf("%s %s / Built on: %s at %s\n", KERNEL_NAME, KERNEL_VERSION, KERNEL_DATE, KERNEL_TIME);

    isr_init();
    irq_init();

    // enable scheduler
    timer_init(50);

    // enable serial port
    serial_init(SERIAL_COM1, SERIAL_SPEED_115200);
    DEBUG("%s has started\n", KERNEL_NAME);

    keyboard_init();

    // this is needed to compute the values after
    dump_multiboot_info(addr);

    uint64_t mb_start = get_multiboot_start();
    uint64_t mb_end = get_multiboot_end();
    uint64_t k_start = get_kernel_start();
    uint64_t k_end = get_kernel_end();

    DEBUG("multiboot_start = 0x%X, multiboot_end = 0x%X", mb_start, mb_end);
    DEBUG("kernel_start = 0x%X, kernel_end = 0x%X", k_start, k_end);

    // memory
    multiboot_tag_mmap_t *mmap = find_multiboot_tag(addr, MULTIBOOT_TAG_TYPE_MMAP);
    mmap_init(mmap, k_start, k_end, addr, (addr + *(unsigned *) addr));

    while (1) {
        __asm__("hlt");
    }
}