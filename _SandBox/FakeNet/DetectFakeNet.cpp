#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <string>
    
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
            }
            InternetCloseHandle(hConnect);
        }
    }

    InternetCloseHandle(hInternet);
    return result;
}

int main() {
    Sleep(30000);
    if (IsConnectedToInternet()) {
        MessageBox(NULL, "Connected normally", "Safe", MB_OK);
        MessageBox(NULL, "enter your malicious code here", "hahaha", MB_OK);
    } else {
        MessageBox(NULL, "FAKENET DETECTED", "DANGER", MB_OK);
        ExitProcess(1);
    }
    return 0;
}
