#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <tchar.h>

int main() {
    HMODULE hKernel32 = LoadLibraryA("kernel32.dll");                                                      // dynamically load the kernel32 DLL

// obfuscate IAT
  
    FARPROC pProcess32First = GetProcAddress(hKernel32, "Process32First");
    typedef BOOL (WINAPI *Process32First_t)(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);
    Process32First_t pProcess32Firstfun = (Process32First_t)pProcess32First;

    FARPROC pCreateToolhelp32Snapshot = GetProcAddress(hKernel32, "CreateToolhelp32Snapshot");
    typedef HANDLE (WINAPI *CreateToolhelp32Snapshot_t)(DWORD dwFlags, DWORD th32ProcessID);
    CreateToolhelp32Snapshot_t pCreateToolhelp32Snapshotfunc = (CreateToolhelp32Snapshot_t)pCreateToolhelp32Snapshot;

    FARPROC pProcess32Next = GetProcAddress(hKernel32, "Process32Next");
    typedef BOOL (WINAPI *Process32Next_t)(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);
    Process32Next_t pProcess32Nextfunc = Process32Next_t(pProcess32Next);

    const TCHAR* targetProcess = _T("Notepad.exe");

    std::wcout << L"Waiting for " << targetProcess << L" to start...\n";

// Check for Notepad.exe
  
    while (true) {
        
        bool exists = false;
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);

        HANDLE snapshot = pCreateToolhelp32Snapshotfunc(TH32CS_SNAPPROCESS, NULL);

        if (pProcess32Firstfun(snapshot, &entry)) {
            do {
                if (_tcscmp(entry.szExeFile, targetProcess) == 0) {
                    exists = true;
                    break;
                }
            } while (pProcess32Nextfunc(snapshot, &entry));
        }

        CloseHandle(snapshot);

        if (exists == true) {
            break;
        }
        Beep(1000, 1000); // Check every second 
    }

    std::wcout << targetProcess << L" is now running. Continuing execution...\n";

    return 0;
}
