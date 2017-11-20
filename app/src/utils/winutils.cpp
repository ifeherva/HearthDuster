//
// winutils.cpp
//  Windows utils and helper functions
//
//  Created by Istvan Fehervari on 01/05/2017.
//  Copyright © 2017 com.ifehervari. All rights reserved.
//

#include <windows.h>
#include <tlhelp32.h>

DWORD GetProcId(const WCHAR* ProcName) {
    PROCESSENTRY32   pe32;
    HANDLE         hSnapshot = NULL;

    pe32.dwSize = sizeof(PROCESSENTRY32);
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    bool gameRuns = false;

    if (Process32First(hSnapshot, &pe32))
    {
        do {
            if (wcscmp(pe32.szExeFile, ProcName) == 0)
            {
                gameRuns = true;
                break;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }

    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hSnapshot);
    }

    if (!gameRuns) return 0;

    return pe32.th32ProcessID;
}

int getHearthstonePID() {

    return GetProcId(L"Hearthstone.exe");
}
