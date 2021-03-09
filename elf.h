#define ELF_MAGIC 0x464C457FU  // "\x7FELF" in little endian


// File header
struct elfhdr {

  uint magic;  // 必须等同于上面的ELF_MAGIC  魔数必须固定 4个字节
  uchar elf[12]; 


  ushort type; //elf文件类型  三种
  ushort machine; //CPU平台属性
  uint version; //ELF版本
  uint entry;   //虚拟地址 重定位文件为0
  uint phoff;   //
  uint shoff;   //段表位置
  uint flags;   //标志位 用来表示elf的相关属性
  ushort ehsize;    //elf文件的大小
  ushort phentsize; //
  ushort phnum;     //
  ushort shentsize; //段描述符的大小
  ushort shnum;     //段描述符的数量
  ushort shstrndx;  //字符串表的下标
};

// Program section header
struct proghdr {
  uint type;    //Segment程序的类型 是装载 还是动态链接
  uint off;     //Segment偏移
  uint vaddr;   //Segment虚拟地址空间的起始位置 LOAD按小->大排序
  uint paddr;   //Segment的物理装在地址LMA
  uint filesz;
  uint memsz;
  uint flags;
  uint align;
};