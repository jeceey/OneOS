#include <stdint.h>

void putchar(char c);

struct idt_entry_struct {
    uint16_t base_low;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr_struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct idt_entry_struct idt_entries[256];
struct idt_ptr_struct   idt_ptr;

extern void idt_load(uint32_t);
extern void isr0();
extern void idt_load(uint32_t);
extern void irq0();
extern void irq1();

void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void remap_pic() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20); 
    outb(0xA1, 0x28); 
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);  
    outb(0xA1, 0x0);
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].base_low  = (base & 0xFFFF);
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].sel       = sel;
    idt_entries[num].always0   = 0;
    idt_entries[num].flags     = flags;
}

struct registers {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

const char keyboard_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', ')', '-', '\b', 
    '\t', 'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '^', '$', '\n', 
    0, 'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'ù', '²', 
    0, '*', 'w', 'x', 'c', 'v', 'b', 'n', ',', ';', ':', '!', 0, 
    '*', 0, ' ', 0
};

void isr_handler(struct registers regs) {
    char *video_memory = (char*) 0xB8000;
    if (regs.int_no == 0) {
        const char *err = "CRITICAL: DIVISION BY ZERO!";
        for(int i=0; err[i] != '\0'; i++) {
            video_memory[40*2 + i*2] = err[i];
            video_memory[40*2 + i*2 + 1] = 0x4F;
        }
    }
}

void irq_handler(struct registers regs) {
    if (regs.int_no == 33) {
        uint8_t scancode = inb(0x60);
        
        if (!(scancode & 0x80)) {
            char ascii = keyboard_map[scancode];
            if (ascii != 0) {
                putchar(ascii);
            }
        }
    }

    if (regs.int_no >= 40) {
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);
}

void init_idt() {
    idt_ptr.limit = (sizeof(struct idt_entry_struct) * 256) - 1;
    idt_ptr.base  = (uint32_t)&idt_entries;

    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    remap_pic();

    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);  
    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E); 
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E); 

    idt_load((uint32_t)&idt_ptr);
}