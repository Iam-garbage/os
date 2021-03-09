#include "asm.h"
#include "mmu.h"
.code 16 ;最开始要从实模式切换到保护模式
.globl start ;开始

start:
    cli     ;禁止BIOS中断

    ;初始化
    xorw %ax,%ax ;归零！
    movw %ax,%ds
    movw %ax,%es 
    movw %ax,%ss

    # 加载LGDT表
    lgdt gdtdesc

    # 打开cr0 并且将PE置位
    movl    %cr0, %eax
    orl     $CR0_PE, %eax
    movl    %eax, %cr0

    # jmp far cs:eip
    # 2^3
    ljmp    $(SEG_KCODE<<3),$start

.code 32
start32:
# 初始化各个字段
    movw    $(SEG_KDATA<<3), %ax
    movw    %ax, %ds
    movw    %ax, %es
    movw    %ax, %ss
    movw    $0, %ax                 
    movw    %ax, %fs                
    movw    %ax, %gs     

# 为C语言设置好栈  
    movl    $start, %esp
# 剩下的事情给C做了
    call    bootmain           
.p2align 2  ;强制4字节对齐
gdt:
    SEG_NULLASM                             # null seg  #0  gdt0号字段必须为空
    SEG_ASM(STA_X|STA_R, 0x0, 0xffffffff)   # code seg  #1  gdt1号为cs段
    SEG_ASM(STA_W, 0x0, 0xffffffff)         # data seg  #2  gdt2号为ds段


gdtdesc:
    .word (gdtdesc-gdt-1)
    .long gdt