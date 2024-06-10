Here we look at 4 techniques that have been used to evade `sandbox` detections and `debuggers`,

1. FakeNet detection
2. INT 3 instruction - Anti-Debug
3. RDTSC check using CPUID
4. Sleep until a process is launched --> (using Beep)

The explanation of individual methods have been given in the respective directories. 

If any of these checks fail, the process is terminated and the binary leaves.
