#include <windows.h>
#include <psapi.h>
#include <iomanip>
#include <iostream>

const int UNIT_KB = 1024;
const int UNIT_MB = 1204 * 1024;
const int UNIT_GB = 1024 * 1024 * 1024;
const int WIDTH = 35;

using namespace std;

#pragma comment(lib, "psapi.lib")

int main(int argc, char const *argv[])
{
  system("cls");
  while (1)
  {
    // Retrieves information about the system's current usage of both physical and virtual memory.
    // 获取系统当前物理存储和虚拟存储的信息
    MEMORYSTATUSEX stat;
    stat.dwLength = sizeof(stat);
    GlobalMemoryStatusEx(&stat);

    cout << setw(WIDTH) << left << "------------- MEMORY -------------" << endl;
    cout << setw(WIDTH) << left << "Memory in use: " << fixed << setprecision(2) << (float)stat.dwMemoryLoad << " %" << endl;
    cout << setw(WIDTH) << left << "Total physical memory: " << fixed << setprecision(2) << (float)stat.ullTotalPhys / UNIT_GB << " GB" << endl;
    cout << setw(WIDTH) << left << "Available physical memory: " << fixed << setprecision(2) << (float)stat.ullAvailPhys / UNIT_GB << " GB" << endl;
    cout << setw(WIDTH) << left << "Total paging file: " << fixed << setprecision(2) << (float)stat.ullTotalPageFile / UNIT_GB << " GB" << endl;
    cout << setw(WIDTH) << left << "Available paging file: " << fixed << setprecision(2) << (float)stat.ullAvailPageFile / UNIT_GB << " GB" << endl;
    cout << setw(WIDTH) << left << "Total virtual memory: " << fixed << setprecision(2) << (float)stat.ullTotalVirtual / UNIT_GB << " GB" << endl;
    cout << setw(WIDTH) << left << "Available virtual memory: " << fixed << setprecision(2) << (float)stat.ullAvailVirtual / UNIT_GB << " GB" << endl;

    // Contains information about the current computer system.
    // 当前计算机系统的信息
    SYSTEM_INFO sysi;
    ZeroMemory(&sysi, sizeof(sysi));
    GetSystemInfo(&sysi);

    cout << setw(WIDTH) << left << "---------- SYSTEM INFO -----------" << endl;
    cout << setw(WIDTH) << left << "Page size: " << sysi.dwPageSize / UNIT_KB << " KB" << endl;
    cout << setw(WIDTH) << left << "Lowest memory address accessible: " << sysi.lpMinimumApplicationAddress << endl;
    cout << setw(WIDTH) << left << "Highest memory address accessible: " << sysi.lpMaximumApplicationAddress << endl;
    cout << setw(WIDTH) << left << "Allocation granularity: " << sysi.dwAllocationGranularity / UNIT_KB << " KB" << endl;

    // Gets performance information.
    PERFORMANCE_INFORMATION performInfo;
    performInfo.cb = sizeof(performInfo);
    GetPerformanceInfo(&performInfo, sizeof(performInfo));

    cout << setw(WIDTH) << left << "---- PERFORMANCE INFORMATION -----" << endl;
    cout << setw(WIDTH) << left << "Total commit pages: " << performInfo.CommitTotal << endl;
    cout << setw(WIDTH) << left << "Current max commit pages" << performInfo.CommitLimit << endl;
    cout << setw(WIDTH) << left << "Peak commit pages: " << performInfo.CommitPeak << endl;
    cout << setw(WIDTH) << left << "Actual physical memory: " << performInfo.PhysicalTotal << endl;
    cout << setw(WIDTH) << left << "Available physical memory: " << performInfo.PhysicalAvailable << endl;
    cout << setw(WIDTH) << left << "System cache: " << performInfo.SystemCache << endl;
    cout << setw(WIDTH) << left << "Kernel total memory: " << performInfo.KernelTotal << endl;
    cout << setw(WIDTH) << left << "Kernel paged memory: " << performInfo.KernelPaged << endl;
    cout << setw(WIDTH) << left << "Kernel nonpaged memory: " << performInfo.KernelNonpaged << endl;
    cout << setw(WIDTH) << left << "Page size: " << performInfo.PageSize / UNIT_KB << " KB" << endl;
    cout << setw(WIDTH) << left << "Open handles: " << performInfo.HandleCount << endl;
    cout << setw(WIDTH) << left << "Processes: " << performInfo.ProcessCount << endl;
    cout << setw(WIDTH) << left << "Threads: " << performInfo.ThreadCount << endl;

    Sleep(1000);
    system("cls");
  }

  return 0;
}
