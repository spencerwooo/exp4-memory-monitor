#include <windows.h>
#include <psapi.h>
#include <iomanip>
#include <iostream>

const int UNIT_KB = 1024;
const int UNIT_MB = 1204 * 1024;
const int UNIT_GB = 1024 * 1024 * 1024;
const int COL_1 = 35;
const int COL_2 = 25;
const int BLOCK_1 = 1;
const int BLOCK_2 = 7;
const int BLOCK_3 = 12;

// FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE |
const unsigned short BLUE_TITLE = BACKGROUND_BLUE;
const unsigned short YELLOW_TITLE = BACKGROUND_RED | BACKGROUND_GREEN;
const unsigned short GREEN_TITLE = BACKGROUND_GREEN;
const unsigned short RED_TITLE = BACKGROUND_RED;
const unsigned short BLUE_CONTENT = FOREGROUND_BLUE;
const unsigned short GREEN_CONTENT = FOREGROUND_GREEN;
const unsigned short RED_CONTENT = FOREGROUND_RED;
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

void printTitle(string title)
{
  int Width = COL_1 + COL_2 - title.length();
  int halfWidth = Width / 2;
  for (int i = 0; i < halfWidth; i++)
  {
    cout << " ";
  }
  cout << title;
  for (int i = 0; i < Width - halfWidth; i++)
  {
    cout << " ";
  }
  cout << endl;
}

int main(int argc, char const *argv[])
{
  HANDLE hStdout;
  hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
  clearScreen(hStdout);

  setConsoleColor(BLUE_TITLE);
  printTitle("MEMORY");
  setConsoleColor(BLUE_CONTENT);
  cout << "Memory in use: " << endl;
  cout << endl;
  cout << "Physical memory: " << endl;
  cout << "Paging file: " << endl;
  cout << "Virtual memory: " << endl;

  setConsoleColor(YELLOW_TITLE);
  printTitle("SYSTEM INFO");
  setConsoleColor(YELLOW_CONTENT);
  cout << "Page size: " << endl;
  cout << "Lowest memory address accessible: " << endl;
  cout << "Highest memory address accessible: " << endl;
  cout << "Allocation granularity: " << endl;

  setConsoleColor(GREEN_TITLE);
  printTitle("PERFORMANCE INFO");
  setConsoleColor(GREEN_CONTENT);
  cout << "Commit pages: " << endl;
  cout << endl;
  cout << "Physical memory: " << endl;
  cout << "System cache: " << endl;
  cout << "Kernel memory: " << endl;
  cout << "Page size: " << endl;
  cout << "Open handles: " << endl;
  cout << "Processes: " << endl;
  cout << "Threads: " << endl;

  while (1)
  {
    ShowConsoleCursor(false);
    // Retrieves information about the system's current usage of both physical and virtual memory.
    // 获取系统当前物理存储和虚拟存储的信息
    MEMORYSTATUSEX stat;
    stat.dwLength = sizeof(stat);
    GlobalMemoryStatusEx(&stat);

    setConsoleColor(WHITE_CONTENT);

    setCursorPosition(0, BLOCK_1 + 1);
    int memPercentFull = COL_1 - 2;
    int memPercentCurr = memPercentFull * stat.dwMemoryLoad / 100;
    for (int i = 0; i < memPercentFull; i++)
    {
      if (i < memPercentCurr)
      {
        setConsoleColor(RED_CONTENT);
      }
      else
      {
        setConsoleColor(WHITE_CONTENT);
      }
      cout << "|";
    }

    setCursorPosition(COL_1, BLOCK_1 + 1);
    cout << fixed << setprecision(2) << (float)stat.dwMemoryLoad << " %";

    setCursorPosition(COL_1, BLOCK_1 + 2);
    cout << fixed << setprecision(2) << (float)stat.ullAvailPhys / UNIT_GB << " GB";
    cout << "/" << fixed << setprecision(2) << (float)stat.ullTotalPhys / UNIT_GB << " GB";

    setCursorPosition(COL_1, BLOCK_1 + 3);
    cout << fixed << setprecision(2) << (float)stat.ullAvailPageFile / UNIT_GB << " GB";
    cout << "/" << fixed << setprecision(2) << (float)stat.ullTotalPageFile / UNIT_GB << " GB";

    setCursorPosition(COL_1, BLOCK_1 + 4);
    cout << fixed << setprecision(2) << (float)stat.ullAvailVirtual / UNIT_GB << " GB";
    cout << "/" << fixed << setprecision(2) << (float)stat.ullTotalVirtual / UNIT_GB << " GB";

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

    setCursorPosition(0, BLOCK_3 + 1);
    int commitPercentFull = COL_1 - 2;
    int commitPercentCurr = commitPercentFull * performInfo.CommitTotal / performInfo.CommitLimit;
    int commitPercentPeak = commitPercentFull * performInfo.CommitPeak / performInfo.CommitLimit;
    for (int i = 0; i < commitPercentFull; i++)
    {

      if (i < commitPercentCurr)
      {
        setConsoleColor(GREEN_CONTENT);
      }
      else if (i >= commitPercentCurr && i < commitPercentPeak)
      {
        setConsoleColor(RED_CONTENT);
      }
      else
      {
        setConsoleColor(WHITE_CONTENT);
      }
      cout << "|";
    }

    setCursorPosition(commitPercentPeak - 1, BLOCK_3);
    setConsoleColor(RED_CONTENT);

    cout << u8"\u2193";
    cout << " PEAK: " << performInfo.CommitPeak;

    setCursorPosition(COL_1, BLOCK_3 + 1);
    setConsoleColor(WHITE_CONTENT);
    cout << performInfo.CommitTotal << "/";
    cout << performInfo.CommitLimit;
    setCursorPosition(commitPercentPeak, BLOCK_3);

    setCursorPosition(COL_1, BLOCK_3 + 2);
    cout << performInfo.PhysicalAvailable << "/" << performInfo.PhysicalTotal << " pages";
    setCursorPosition(COL_1, BLOCK_3 + 3);
    cout << performInfo.SystemCache;

    setCursorPosition(COL_1, BLOCK_3 + 4);
    cout << performInfo.KernelPaged << "/" << performInfo.KernelTotal << " pages";
    setCursorPosition(COL_1, BLOCK_3 + 5);
    cout << performInfo.PageSize / UNIT_KB << " KB";
    setCursorPosition(COL_1, BLOCK_3 + 6);
    cout << performInfo.HandleCount;
    setCursorPosition(COL_1, BLOCK_3 + 7);
    cout << performInfo.ProcessCount;
    setCursorPosition(COL_1, BLOCK_3 + 8);
    cout << performInfo.ThreadCount;

    Sleep(1000);
  }

  clearScreen(hStdout);

  return 0;
}
