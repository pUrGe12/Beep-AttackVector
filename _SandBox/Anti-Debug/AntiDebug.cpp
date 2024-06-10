#include <iostream>
#include <iomanip>
#include <windows.h>

void MutateBehaviour(){
    ExitProcess(1);
};

int MaliciousBehaviour(){
    MessageBox(NULL,"entering the malicious code", "haha", MB_OK);
    return 0;
};

int main() {
    int i, j;
    unsigned char* p = reinterpret_cast<unsigned char*>(main);
  
    for (j = 0; j < 4; j++) {
        std::cout << static_cast<void*>(p) << ": ";
        for (i = 0; i < 16; i++) {
            if (*p == 0xCC) {
                MessageBox(NULL, "Debugger Present", "", MB_OK);
                MutateBehaviour();
            } else {
                std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(*p) << " ";
            }

            p++;
        }

        std::cout << std::endl;
    }
    MaliciousBehaviour();
    return 0;
}
