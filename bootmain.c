#include "elf.h"
#include "types.h"
#include "x86.h"

#define SECTSIZE  512 //0x200






void bootmain(void)
{
    struct elfhdr *elf;//exe文件
    struct proghdr *ph, *eph;
    //函数指针 参数无了
    void (*entry)(void);
    uchar* pa;

    //65536  0-65535 ffff
    elf=(struct elfhdr*)0x10000; //分配elf文件位置
    readseg((uchar*)elf, 4096, 0); //将elf文件放置0x10000-0x11000  刚好4096字节

}



// 读取主磁盘
// 读取一个位于offset的扇区放入dst里面

// 主硬盘分配端口号为 0x1f0-0x1f7 一共八位
// 副硬盘分配端口号为 0x170-0x177 一共八位
void readsect(void *dst, uint offset)
{
  // Issue command.
  waitdisk();
  outb(0x1F2, 1);   // count = 1
  outb(0x1F3, offset);
  outb(0x1F4, offset >> 8);
  outb(0x1F5, offset >> 16);
  outb(0x1F6, (offset >> 24) | 0xE0);
  outb(0x1F7, 0x20);  // cmd 0x20 - read sectors

  // Read data.
  waitdisk();
  insl(0x1F0, dst, SECTSIZE/4);
}

//在偏移处offset读取count个字节数量 然后装载到pa指向的物理内存
//pa=0x10000
//count=0x1000
//offset=0
//pa+count -> 0x10000+0x1000=0x110000
void readseg(uchar* pa, uint count, uint offset)
{
    uchar* epa;
    
    epa=pa+count;//epa=0x11000

    //下调至sector边界 存访不是kernel文件的东西
    //pa = 0x10000-0x10000/0x200
    pa -= offset % SECTSIZE;

    //将kernel放在第一个扇区
    offset=(offset/SECTSIZE)+1;

    //按照每512字节读 先读取第一个扇区 一共八个 0x200*8=0x1000
    for(; pa < epa; pa += SECTSIZE, offset++)
      readsect(pa, offset);
}