#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <conio.h>
#include <iostream>
#include <string>

struct ProcessInfo {
    STARTUPINFO startupinfo;
    PROCESS_INFORMATION processInformation;
};

int main() {
    TCHAR Name[] = "CriticalSection";
    HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(CRITICAL_SECTION), Name);
    if (hMapFile == NULL) {
        printf("Can't create file mapping.\n");
        return -1;
    }

    CRITICAL_SECTION* criticalSection = (CRITICAL_SECTION*)MapViewOfFile(
            hMapFile,FILE_MAP_ALL_ACCESS, 0, 0, sizeof(CRITICAL_SECTION));
    if (criticalSection == NULL) {
        printf("Can't map view of file.\n");
        CloseHandle(hMapFile);
        return -1;
    }

    InitializeCriticalSection(criticalSection);

    ProcessInfo processInfos[2];
    for (int i = 0; i < 2; i++) {
        ZeroMemory(&processInfos[i], sizeof(ProcessInfo));
        processInfos[i].startupinfo.cb = sizeof(STARTUPINFO);
        if (!CreateProcess("B:\\OSaSP\\CriticalSection\\TestProcess1\\cmake-build-debug\\TestProcess1.exe",
                           NULL, 0, 0, TRUE, 0, 0, NULL, &processInfos[i].startupinfo,
                           &processInfos[i].processInformation)) {
            printf("Can't create process.\n");
        }
    }

    for (int i = 0; i < 2; i++) {
        WaitForSingleObject(processInfos[i].processInformation.hProcess, INFINITE);
        CloseHandle(processInfos[i].processInformation.hProcess);
        CloseHandle(processInfos[i].processInformation.hThread);
    }

    DeleteCriticalSection(criticalSection);
    UnmapViewOfFile(criticalSection);
    CloseHandle(hMapFile);
}
