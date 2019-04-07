#include <windows.h>
#include <psapi.h>
#include <iomanip>
#include <iostream>
#include <tlhelp32.h>
#include <string.h>
#include <shlwapi.h>
#include <conio.h>

// 单位转化
const int UNIT_KB = 1024;
const int UNIT_MB = 1204 * 1024;
const int UNIT_GB = 1024 * 1024 * 1024;
// 表格行列定位
const int COL_1 = 35;
const int COL_2 = 25;
const int COL_3 = 80;
const int COL_2_1 = 28;
const int COL_2_2 = 11;
const int BLOCK_1 = 1;
const int BLOCK_2 = 7;
const int BLOCK_3 = 12;
const int BLOCK_4 = 9;
// 配色方案
const unsigned short BLUE_TITLE = BACKGROUND_BLUE;
const unsigned short YELLOW_TITLE = BACKGROUND_RED | BACKGROUND_GREEN;
const unsigned short GREEN_TITLE = BACKGROUND_GREEN;
const unsigned short RED_TITLE = BACKGROUND_RED;
const unsigned short WHITE_TITLE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
const unsigned short BLUE_CONTENT = FOREGROUND_BLUE;
const unsigned short GREEN_CONTENT = FOREGROUND_GREEN;
const unsigned short RED_CONTENT = FOREGROUND_RED;
const unsigned short YELLOW_CONTENT = FOREGROUND_RED | FOREGROUND_GREEN;
const unsigned short WHITE_CONTENT = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE;

using namespace std;

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "shlwapi.lib")

// 隐藏光标位置，防止刷新频闪
void ShowConsoleCursor(bool showFlag)
{
  HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

  CONSOLE_CURSOR_INFO cursorInfo;

  GetConsoleCursorInfo(out, &cursorInfo);
  cursorInfo.bVisible = showFlag; // set the cursor visibility
  SetConsoleCursorInfo(out, &cursorInfo);
}

// 清屏，即 system("cls") 的实现
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

// 设置光标输出位置
void setCursorPosition(int x, int y)
{
  static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  cout.flush();
  COORD coord = {(SHORT)x, (SHORT)y};
  SetConsoleCursorPosition(hOut, coord);
}

// 设置输出颜色
void setConsoleColor(unsigned short color)
{
  static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  cout.flush();
  SetConsoleTextAttribute(hOut, color);
}

// 输出表格标题栏
void printTitle(string title, int titleLength)
{
  int Width = titleLength - title.length();
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
  // 清屏
  HANDLE hStdout;
  hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
  clearScreen(hStdout);

  // 打印表格中的标题列和不动的部分
  setConsoleColor(BLUE_TITLE);
  printTitle("MEMORY", COL_1 + COL_2);
  setConsoleColor(BLUE_CONTENT);
  cout << "Memory in use: " << endl;
  cout << endl;
  cout << "Physical memory: " << endl;
  cout << "Paging file: " << endl;
  cout << "Virtual memory: " << endl;

  setConsoleColor(YELLOW_TITLE);
  printTitle("SYSTEM INFO", COL_1 + COL_2);
  setConsoleColor(YELLOW_CONTENT);
  cout << "Page size: " << endl;
  cout << "Lowest memory address accessible: " << endl;
  cout << "Highest memory address accessible: " << endl;
  cout << "Allocation granularity: " << endl;

  setConsoleColor(GREEN_TITLE);
  printTitle("PERFORMANCE INFO", COL_1 + COL_2);
  setConsoleColor(GREEN_CONTENT);
  cout << "Commit pages: " << endl;
  cout << "\n"
       << endl;
  cout << "Physical memory: " << endl;
  cout << "System cache: " << endl;
  cout << "Kernel memory: " << endl;
  cout << "Page size: " << endl;
  cout << "Open handles: " << endl;
  cout << "Processes: " << endl;
  cout << "Threads: " << endl;

  setConsoleColor(WHITE_TITLE);
  printTitle("Press any key to exit.", COL_1 + COL_2 + COL_3);

  setCursorPosition(COL_1 + COL_2, 0);
  setConsoleColor(RED_TITLE);
  printTitle("PROCESS INFO", COL_3);

  setCursorPosition(COL_1 + COL_2 + 1, 1);
  setConsoleColor(RED_CONTENT);
  cout << setw(COL_2_1) << left << "Process ID";
  cout << setw(COL_2_1) << left << "Process Name";
  cout << setw(COL_2_1) << left << "Working Set Size" << endl;

  if (argc == 2)
  {
    setCursorPosition(COL_1 + COL_2, BLOCK_4);

    char procDetailTitle[260];
    sprintf(procDetailTitle, "Process <%s> detail", argv[1]);

    setConsoleColor(RED_TITLE);
    printTitle(procDetailTitle, COL_3);

    setConsoleColor(RED_CONTENT);
    setCursorPosition(COL_1 + COL_2 + 1, BLOCK_4 + 1);
    cout << setw(COL_2_2 + 24) << left << "Block Address";
    cout << setw(COL_2_2) << left << "Block Stat";
    cout << setw(COL_2_2) << left << "Protection";
    cout << setw(COL_2_2) << left << "Type";
    cout << setw(COL_2_2) << left << "EXE name";
  }

  while (!_kbhit())
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

    setCursorPosition(0, BLOCK_3 + 2);
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

    setCursorPosition(0, BLOCK_3 + 1);
    for (int i = 0; i < commitPercentPeak - 1; i++)
    {
      cout << " ";
    }
    setConsoleColor(RED_CONTENT);

    cout << u8"\u2193";
    cout << " PEAK: " << performInfo.CommitPeak;

    setCursorPosition(COL_1, BLOCK_3 + 2);
    setConsoleColor(WHITE_CONTENT);
    cout << performInfo.CommitTotal << "/";
    cout << performInfo.CommitLimit;
    setCursorPosition(commitPercentPeak, BLOCK_3);

    setCursorPosition(COL_1, BLOCK_3 + 3);
    cout << performInfo.PhysicalAvailable << "/" << performInfo.PhysicalTotal << " pages";
    setCursorPosition(COL_1, BLOCK_3 + 4);
    cout << performInfo.SystemCache;

    setCursorPosition(COL_1, BLOCK_3 + 5);
    cout << performInfo.KernelPaged << "/" << performInfo.KernelTotal << " pages";
    setCursorPosition(COL_1, BLOCK_3 + 6);
    cout << performInfo.PageSize / UNIT_KB << " KB";
    setCursorPosition(COL_1, BLOCK_3 + 7);
    cout << performInfo.HandleCount;
    setCursorPosition(COL_1, BLOCK_3 + 8);
    cout << performInfo.ProcessCount;
    setCursorPosition(COL_1, BLOCK_3 + 9);
    cout << performInfo.ThreadCount;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(pe);
    HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    BOOL bMore = ::Process32First(hProcessSnap, &pe);

    int i = 0;
    DWORD pid = 0;
    while (bMore)
    {
      HANDLE hP = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);
      PROCESS_MEMORY_COUNTERS pmc;
      ZeroMemory(&pmc, sizeof(pmc));
      GetProcessMemoryInfo(hP, &pmc, sizeof(pmc));

      pid = pe.th32ProcessID;

      if (argc != 2)
      {
        setCursorPosition(COL_1 + COL_2 + 1, i + 2);
        cout << "PID: " << pe.th32ProcessID;
        setCursorPosition(COL_1 + COL_2 + 1 + COL_2_1, i + 2);
        cout << pe.szExeFile << endl;
        setCursorPosition(COL_1 + COL_2 + 1 + COL_2_1 + COL_2_1, i + 2);
        cout << pmc.WorkingSetSize / UNIT_MB << "MB" << endl;

        i++;

        if (i > 19)
          break;
      }
      else
      {
        char procName[260];
        sprintf(procName, "%s", pe.szExeFile);

        char queryProcName[260];
        sprintf(queryProcName, "%s", argv[1]);

        if (!strcmp(procName, queryProcName))
        {
          setCursorPosition(COL_1 + COL_2 + 1, i + 2);
          cout << "PID: " << pe.th32ProcessID;
          setCursorPosition(COL_1 + COL_2 + 1 + COL_2_1, i + 2);
          cout << pe.szExeFile << endl;
          setCursorPosition(COL_1 + COL_2 + 1 + COL_2_1 + COL_2_1, i + 2);
          cout << pmc.WorkingSetSize / UNIT_MB << "MB" << endl;

          i++;

          if (i > 5)
            break;
        }
      }

      bMore = ::Process32Next(hProcessSnap, &pe);
    }

    // 输出进程虚拟地址信息
    if (argc == 2)
    {
      // setCursorPosition(COL_1 + COL_2 + 1, BLOCK_4 + 1);
      // cout << "[Process details here:]";

      HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
      MEMORY_BASIC_INFORMATION mbi;  // 进程虚拟内存空间的基本信息结构
      ZeroMemory(&mbi, sizeof(mbi)); // 分配缓冲区，用于保存信息

      LPCVOID pBlock = (LPVOID)sysi.lpMinimumApplicationAddress;
      for (int i = 0; i < 10; i++)
      {
        if (VirtualQueryEx(hProc, pBlock, &mbi, sizeof(mbi)) == sizeof(mbi))
        {
          LPCVOID pEnd = (PBYTE)pBlock + mbi.RegionSize;
          TCHAR szSize[MAX_PATH];
          StrFormatByteSize(mbi.RegionSize, szSize, MAX_PATH);
          setCursorPosition(COL_1 + COL_2 + 1, BLOCK_4 + 2 + i);
          cout << pBlock << "-" << pEnd << "(" << szSize << ")";

          setCursorPosition(COL_1 + COL_2 + 1 + COL_2_2 + 24, BLOCK_4 + 2 + i);
          switch (mbi.State)
          {
          case MEM_COMMIT:
            cout << "Commited ";
            break;
          case MEM_FREE:
            cout << "Free     ";
            break;
          case MEM_RESERVE:
            cout << "Reserved ";
            break;
          default:
            cout << "-        ";
          }

          setCursorPosition(COL_1 + COL_2 + 1 + COL_2_2 * 2 + 24, BLOCK_4 + 2 + i);
          cout << mbi.Protect;
          // if (mbi.Protect == 0 && mbi.State != MEM_FREE)
          // {
          //   mbi.Protect = PAGE_READONLY;
          // }

          setCursorPosition(COL_1 + COL_2 + 1 + COL_2_2 * 3 + 24, BLOCK_4 + 2 + i);
          switch (mbi.Type)
          {
          case MEM_IMAGE:
            cout << "Image  ";
            break;
          case MEM_MAPPED:
            cout << "Mapped ";
            break;
          case MEM_PRIVATE:
            cout << "Private";
            break;
          default:
            cout << "-      ";
          }

          TCHAR szFilename[MAX_PATH];
          GetModuleFileName((HMODULE)pBlock, szFilename, MAX_PATH);
          PathStripPath(szFilename);
          setCursorPosition(COL_1 + COL_2 + 1 + COL_2_2 * 4 + 24, BLOCK_4 + 2 + i);
          cout << szFilename;
          cout << endl;

          pBlock = pEnd;
        }
      }
    }

    setConsoleColor(WHITE_CONTENT);
    Sleep(1000);
  }

  _getch();
  cout.flush();
  clearScreen(hStdout);

  return 0;
}
