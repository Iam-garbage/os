//封装了一些汇编操作供C语言使用
#include "types.h"
//out byte  byte=8bit
//port 16位端口号
//data 8位
static inline void outb(ushort port,uchar data)
{
    // 这条指令实际上是 out data,port
    //out dx,ax
    //a=eax ax al
    //d=edx dx dl
    asm volatile("out %0,%1" 
                    : 
                    : "a" (data), "d" (port));
}

