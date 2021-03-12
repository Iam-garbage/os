//封装了一些汇编操作供C语言使用
#include "types.h"

static inline uchar inb(ushort port)
{
    uchar data;
    asm volatile("in %1,%0" 
                    : "=a" (data) 
                    : "d" (port));
    return data;
}


//out byte  byte=8bit
//port 16位端口号
//data 8位
static inline void outb(ushort port,uchar data)
{
    asm volatile("out %0,%1"  
                    :  
                    : "a" (data), "d" (port));
}
 
