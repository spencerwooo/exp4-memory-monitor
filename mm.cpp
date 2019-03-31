#include <windows.h>
#include <psapi.h>
#include <iomanip>
#include <iostream>

const int UNIT_KB = 1024;
const int UNIT_MB = 1204 * 1024;
const int UNIT_GB = 1024 * 1024 * 1024;
const int COL_1 = 35;
const int BLOCK_1 = 1;
const int BLOCK_2 = 9;
const int BLOCK_3 = 14;

// FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE |
const unsigned short BLUE_TITLE = BACKGROUND_BLUE;
const unsigned short YELLOW_TITLE = BACKGROUND_RED | BACKGROUND_GREEN;
const unsigned short GREEN_TITLE = BACKGROUND_GREEN;
const unsigned short BLUE_CONTENT = FOREGROUND_BLUE;
const unsigned short GREEN_CONTENT = FOREGROUND_GREEN;
const unsigned short YELLOW_CONTENT = FOREGROUND_RED | FOREGROUND_GREEN;
const unsigned short WHITE_CONTENT = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE;

using namespace std;

#pragma comment(lib, "psapi.lib")

void ShowConsoleCursor(bool showFlag)
{
  HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

  CONSOLE_CURSOR_INFO cursorInfo;

  GetConsoleCursorInfo(out, &cursorInfo);
  cursorInfo.bVisible = showFlag; // set the cursor visibility
  SetConsoleCursorInfo(out, &cursorInfo);
}

void clearScreen(HANDLE hConsole)
{
  COORD coordScreen = {0, 0};
  DWORD cCharsWritten;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD dwConSize;

  if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
  {
    return;
  }
  dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

  if (!FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten))
  {
    return;
  }

  if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
  {
    return;
  }

  if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten))
  {
    return;
  }

  SetConsoleCursorPosition(hConsole, coordScreen);
}

void setCursorPosition(int x, int y)
{
  static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  cout.flush();
  COORD coord = {(SHORT)x, (SHORT)y};
  SetConsoleCursorPosition(hOut, coord);
}

void setConsoleColor(unsigned short color)
{
  static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  cout.flush();
  SetConsoleTextAttribute(hOut, color);
}

int main(int argc, char const *argv[])
{
  HANDLE hStdout;
  hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
  clearScreen(hStdout);

  setConsoleColor(BLUE_TITLE);
  cout << setw(COL_1) << left << "------------- MEMORY -------------" << endl;
  setConsoleColor(BLUE_CONTENT);
  cout << setw(COL_1) << left << "Memory in use: " << endl;
  cout << setw(COL_1) << left << "Total physical memory: " << endl;
  cout << setw(COL_1) << left << "Available physical memory: " << endl;
  cout << setw(COL_1) << left << "Total paging file: " << endl;
  cout << setw(COL_1) << left << "Available paging file: " << endl;
  cout << setw(COL_1) << left << "Total virtual memory: " << endl;
  cout << setw(COL_1) << left << "Available virtual memory: " << endl;

  setConsoleColor(YELLOW_TITLE);
  cout << setw(COL_1) << left << "---------- SYSTEM INFO -----------" << endl;
  setConsoleColor(YELLOW_CONTENT);
  cout << setw(COL_1) << left << "Page size: " << endl;
  cout << setw(COL_1) << left << "Lowest memory address accessible: " << endl;
  cout << setw(COL_1) << left << "Highest memory address accessible: " << endl;
  cout << setw(COL_1) << left << "Allocation granularity: " << endl;

  setConsoleColor(GREEN_TITLE);
  cout << setw(COL_1) << left << "---- PERFORMANCE INFORMATION -----" << endl;
  setConsoleColor(GREEN_CONTENT);
  cout << setw(COL_1) << left << "Total commit pages: " << endl;
  cout << setw(COL_1) << left << "Current max commit pages" << endl;
  cout << setw(COL_1) << left << "Peak commit pages: " << endl;
  cout << setw(COL_1) << left << "Actual physical memory: " << endl;
  cout << setw(COL_1) << left << "Available physical memory: " << endl;
  cout << setw(COL_1) << left << "System cache: " << endl;
  cout << setw(COL_1) << left << "Kernel total memory: " << endl;
  cout << setw(COL_1) << left << "Kernel paged memory: " << endl;
  cout << setw(COL_1) << left << "Kernel nonpaged memory: " << endl;
  cout << setw(COL_1) << left << "Page size: " << endl;
  cout << setw(COL_1) << left << "Open handles: " << endl;
  cout << setw(COL_1) << left << "Processes: " << endl;
  cout << setw(COL_1) << left << "Threads: " << endl;

  while (1)
  {
    ShowConsoleCursor(false);
    // Retrieves information about the system's current usage of both physical and virtual memory.
    // 获取系统当前物理存储和虚拟存储的信息
    MEMORYSTATUSEX stat;
    stat.dwLength = sizeof(stat);
    GlobalMemoryStatusEx(&stat);

    setConsoleColor(WHITE_CONTENT);

    setCursorPosition(COL_1, BLOCK_1);
    cout << fixed << setprecision(2) << (float)stat.dwMemoryLoad << " %";
    setCursorPosition(COL_1, BLOCK_1 + 1);
    cout << fixed << setprecision(2) << (float)stat.ullTotalPhys / UNIT_GB << " GB";
    setCursorPosition(COL_1, BLOCK_1 + 2);
    cout << fixed << setprecision(2) << (float)stat.ullAvailPhys / UNIT_GB << " GB";
    setCursorPosition(COL_1, BLOCK_1 + 3);
    cout << fixed << setprecision(2) << (float)stat.ullTotalPageFile / UNIT_GB << " GB";
    setCursorPosition(COL_1, BLOCK_1 + 4);
    cout << fixed << setprecision(2) << (float)stat.ullAvailPageFile / UNIT_GB << " GB";
    setCursorPosition(COL_1, BLOCK_1 + 5);
    cout << fixed << setprecision(2) << (float)stat.ullTotalVirtual / UNIT_GB << " GB";
    setCursorPosition(COL_1, BLOCK_1 + 6);
    cout << fixed << setprecision(2) << (float)stat.ullAvailVirtual / UNIT_GB << " GB";

    // Contains information about the current computer system.
    // 当前计算机系统的信息
    SYSTEM_INFO sysi;
    ZeroMemory(&sysi, sizeof(sysi));
    GetSystemInfo(&sysi);

    setCursorPosition(COL_1, BLOCK_2);
    cout << sysi.dwPageSize / UNIT_KB << " KB";
    setCursorPosition(COL_1, BLOCK_2 + 1);
    cout << sysi.lpMinimumApplicationAddress;
    setCursorPosition(COL_1, BLOCK_2 + 2);
    cout << sysi.lpMaximumApplicationAddress;
    setCursorPosition(COL_1, BLOCK_2 + 3);
    cout << sysi.dwAllocationGranularity / UNIT_KB << " KB";

    // Gets performance information.
    PERFORMANCE_INFORMATION performInfo;
    performInfo.cb = sizeof(performInfo);
    GetPerformanceInfo(&performInfo, sizeof(performInfo));

    setCursorPosition(COL_1, BLOCK_3);
    cout << performInfo.CommitTotal;
    setCursorPosition(COL_1, BLOCK_3 + 1);
    cout << performInfo.CommitLimit;
    setCursorPosition(COL_1, BLOCK_3 + 2);
    cout << performInfo.CommitPeak;
    setCursorPosition(COL_1, BLOCK_3 + 3);
    cout << performInfo.PhysicalTotal;
    setCursorPosition(COL_1, BLOCK_3 + 4);
    cout << performInfo.PhysicalAvailable;
    setCursorPosition(COL_1, BLOCK_3 + 5);
    cout << performInfo.SystemCache;
    setCursorPosition(COL_1, BLOCK_3 + 6);
    cout << performInfo.KernelTotal;
    setCursorPosition(COL_1, BLOCK_3 + 7);
    cout << performInfo.KernelPaged;
    setCursorPosition(COL_1, BLOCK_3 + 8);
    cout << performInfo.KernelNonpaged;
    setCursorPosition(COL_1, BLOCK_3 + 9);
    cout << performInfo.PageSize / UNIT_KB << " KB";
    setCursorPosition(COL_1, BLOCK_3 + 10);
    cout << performInfo.HandleCount;
    setCursorPosition(COL_1, BLOCK_3 + 11);
    cout << performInfo.ProcessCount;
    setCursorPosition(COL_1, BLOCK_3 + 12);
    cout << performInfo.ThreadCount;

    Sleep(1000);
  }

  clearScreen(hStdout);

  return 0;
}
