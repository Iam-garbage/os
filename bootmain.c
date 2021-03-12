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


//0x1F7 一共八位 1111 0111
//0xC0  一共八位 1100 0000
//0x40  一共八位 1000 0000

//返回出来的数据 一定是 0000 1000
//最高位为1 表示正在忙碌
//第四位为1 表示准备好了
void waitdisk(void)
{
  while((inb(0x1F7) & 0xC0) != 0x40);
}

// 读取主磁盘
// 读取一个位于offset的扇区放入dst里面

// 主硬盘分配端口号为 0x1f0-0x1f7 一共八位
// 副硬盘分配端口号为 0x170-0x177 一共八位
void readsect(void *dst, uint offset)
{
  waitdisk(); //硬盘在忙吗？
  //读取扇区的数量
  //这属于第一步 这一步要写入0x1f2这个端口 而且是个8位端口 如果为0 就代表要读取256个端口
  outb(0x1F2, 1);   // count = 1    
  
  //现在才是真正的读取磁盘数据
  outb(0x1F3, offset);
  outb(0x1F4, offset >> 8); //每右移1位 就代表 除以2 这里右移8位 那就代表后面全是0 所以只读取0x1F3的数据
  outb(0x1F5, offset >> 16); 

  //0x1F6属于最后四位的LAB 0xE0表示 1110 0000
  //高三位全是1 表明这是个LAB读取模式
  //第五位是0 表明是主盘  1代表从盘
  outb(0x1F6, (offset >> 24) | 0xE0);
  
  //向0x1F7写入0x20 表示这是个请求读盘  0x30是写入
  outb(0x1F7, 0x20);  // cmd 0x20 - read sectors

  // Read data.
  waitdisk(); //硬盘在忙吗？
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