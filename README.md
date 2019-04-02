# Memory Monitor

> 【操作系统课程设计】实验四、内存监视

## 实验要求

### Windows 的实验:

Windows 设计一个内存监视器，要求：

- 实时地显示当前系统中内存的使用情况，包括系统地址空间的布局，物理内存的使用情况；
-	实时显示实验二进程控制（`ParentProcess.exe`）的虚拟地址空间布局和工作集信息 
-	相关的系统调用：`GetSystemInfo`、`VirtualQueryEx`、`VirtualAlloc`、`GetPerformanceInfo`、`GlobalMemoryStatusEx`…

### Linux 实验：

-	用 `top` 命令查看系统，子命令 `P、T、M`
-	用 `ps -A` 查看所有进程，找到 `ProcessParent` 的 `pid`
-	用 `top -p pid` 查看 `ProcessParent` 程序的情况；
-	用 `pmap -d pid` 查看 `ProcessParent` 的内存使用情况

## Windows 下的编译

```powershell
g++ mm.cpp -lpsapi -lshlwapi -o mm
```

需要在编译过程指定链接库 `psapi` 和 `shlwapi` 进行编译。