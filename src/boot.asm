MODULEALIGN equ  1 << 0
MEMINFO     equ  1 << 1
FLAGS       equ  MODULEALIGN | MEMINFO
MAGIC       equ  0x1BADB002
CHECKSUM    equ  -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .text
global _start
extern kernel_main
extern isr_handler
extern irq_handler

_start:
    mov esp, stack_space
    
    call kernel_main

    cli
.hang:
    hlt
    jmp .hang

global gdt_flush
gdt_flush:
    mov eax, [esp+4]  
    lgdt [eax]       

    mov ax, 0x10   
    mov ds, ax        
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush
.flush:
    ret

global idt_load
idt_load:
    mov eax, [esp+4] 
    lidt [eax]        
    ret


global isr0
isr0:
    cli
    push byte 0
    push byte 0
    jmp isr_common_stub

; IRQ 0 : Horloge Système (Timer)
global irq0
irq0:
    cli
    push byte 0
    push byte 32
    jmp irq_common_stub

; IRQ 1 : Clavier
global irq1
irq1:
    cli
    push byte 0
    push byte 33
    jmp irq_common_stub

isr_common_stub:
    pusha                    
    mov ax, ds              
    push eax
    mov ax, 0x10      
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call isr_handler        
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa                  
    add esp, 8             
    sti
    iret                  

irq_common_stub:
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call irq_handler        
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8
    sti
    iret

section .bss
resb 8192               
stack_space: