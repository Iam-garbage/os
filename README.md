## Tyos
    
  参考代码 [xv6](https://github.com/mit-pdos/xv6-public)
## 你可能所需要的阅读操作系统的基本知识
* [汇编](https://github.com/TgeaUs/os/blob/main/Basicknowledge/Basicknowledge.md)
* 操作系统
## 阅读顺序
* 启动盘
    * bootmain.S 是以16为cpu操作模式切换成32位 距离内容初始化了寄存器 + 32位模式保护模式所需要的段 + 一个C语言所需要的栈
    * bootmain.C 是C语言模式开始设置东西 初始化 二进制可执行文件格式(ELF)的初始化位置(0x10000) 把kernel放置第一个扇区



