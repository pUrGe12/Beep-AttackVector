# RDTSC

The `RDTSC` instruction returns the current value of `time-stamp counter` (TSC), which is an `in-CPU 64-bit` counter that usually increases at a constant speed close to the CPU frequency. 

The TSC provides a good approximation of the CPU cycles. It has been revealed that RDTSC-based measurement of the CPU cycles required to `execute the CPUID` instruction is effective for guessing the presence of a virtual machine. 

(Oyama, 2019)

`Yara`, a famous tool for malware analysis and detection, contains rules to detect execution of the RDTSC and CPUID instructions. 

The idea is that,

-	Inside of a virtual machines the CPUID instruction `consumes far more CPU cycles` than real machines. 

-	So, we take the RDTSC measurement before executing CPUID, and then take the value after executing CPUID. Their **difference gives an approximate number of CPU cycles** consumed. If this is greater than a threshold, then we may be inside a VM.

Another way to use the RDTSC value is,
-	Some sandboxes `skip sleeps` . So, if we include in our code a sleep(3600) command and then check the difference in **RDTSC values times 60 times 50** (I mean, we are checking for whether if slept for at least 50 mins). If not, then it’s probably a sandbox. 

However, CPU tick counts can be manipulated and hence, not super trust worthy, but a good and necessary check mostly. Combined with other methods this analysis can be pretty good

# Code explanation

The `GetRdtsc` function is what reads the RDTSC value. The best way I could find was to read it using `assembly codes` inside the CPP program.

-	We define a `uint64_t` datatype for the return value. This is an unsigned 64-bit integer. We also assign an unsigned 32-bit integer to variables low and high. These will be used to store the lower and higher 32-bits each of the RDTSC value.

-	Then this `__asm__` volatile combination tells the compiler that we will be using assembly code inside CPP. The `rdtsc` is the instruction that returns the `TSC`. We split the register values depending on the registers.
  
-	We then combine the lower and higher 32-bit values obtained from the assembly instruction into a **single 64-bit value** (shifts the high value 32-bits to the left effectively placing it in the upper 32 bits of a 64-bit unsigned integer, then we perform an `or` operation between the shifted high and low value to combine them into a single 64-bit value).

We use this function before making a call for the CPUID and then after doing it.

Then, I take the difference and check if its greater than a certain `threshold` --> this is the tricky part because the threshold depends on the processor its running on and mostly needs to be manually checked.

To avoid getting the wrong output, I do the same process a 1000 times and check if at least 500 times, it flagged it as a sandbox. If yes, then it probably is a sandbox. I have set the threshold at 500,000 because it gives a counter value of around 30 to 60 in my laptop. This will have to be adjusted according to the target machines. 

This is how it works in my VM.

![image](https://github.com/pUrGe12/Beep-AttackVector/assets/153343775/d597ffaf-2a52-444e-b87a-c89fc2b57b11)

A lot of sandboxes like **AnyRun** will modify their CPU ticks to be **as close to each other** as possible. We can see in the below screenshot that the difference in RDTSC values before and after CPUID execution is minimal. Thus, our program can also check for **very small differences** in these values because they likely indicate sandbox as well.

![image](https://github.com/pUrGe12/Beep-AttackVector/assets/153343775/b875360a-2ace-4bc3-b29d-42868c0a2185)





