#include <stdint.h>
#include <x86intrin.h>
#include <cpuid.h>
#include <windows.h>
#include <iostream>

uint64_t getRdtsc() {                                                 
  uint32_t low, high;                                                 
  __asm__ volatile ("rdtsc" : "=a" (low), "=d" (high));                
  return ((uint64_t) high << 32) | low;
}

int main() {
  unsigned int eax, ebx, ecx, edx;
  int counter = 0;

  for (int i = 0; i < 1000; i++){
    uint64_t rdtsc_value_a = getRdtsc();
    std::cout << "RDTSC value: " << rdtsc_value_a << std::endl;                               

    __cpuid(0, eax, ebx, ecx, edx);

    uint64_t rdtsc_value_b = getRdtsc();
    std::cout << "RDTSC value: " << rdtsc_value_b << std::endl;

    if (rdtsc_value_b - rdtsc_value_a > 550000){                                                // This value needs to be configured by trial and error
      counter++;
    }
    std::cout << "Iteration " << (i + 1) << ": Counter = " << counter << std::endl;
  }
  if (counter > 5 && counter < 500) {
      MessageBox(NULL, "not a sandbox", "safe", MB_OK);
  } else {
      MessageBox(NULL, "sandbox", "ALERT", MB_OK);
  }
  return 0;
}
