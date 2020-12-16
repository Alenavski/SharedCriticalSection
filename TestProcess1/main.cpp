#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <conio.h>
#include <iostream>
#include <string>

int main() {
    TCHAR Name[] = "CriticalSection";
    HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, Name);
    if (hMapFile == NULL)
    {
        printf("Can't open file mapping.\n");
        return -1;
    }

    CRITICAL_SECTION* criticalSection = (CRITICAL_SECTION*)MapViewOfFile(
            hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(CRITICAL_SECTION));
    if (criticalSection == NULL)
    {
        printf("Can't map view of file.\n");
        CloseHandle(hMapFile);
        return -1;
    }

    bool enteringCriticalSection = false;

    while (!enteringCriticalSection)
    {
        if (TryEnterCriticalSection(criticalSection))
        {
            for (int i = 0; i < 10; i++)
            {
                printf("Hello %d\n", i);
                Sleep(20);
            }
            LeaveCriticalSection(criticalSection);
            enteringCriticalSection = true;
        }
    }

    UnmapViewOfFile(criticalSection);
    CloseHandle(hMapFile);
}
