The encrypted C++ code is a simple `process injection` program. This process is called an `Early-bird injection` because we use `Asynchronous Procedure Calls (APCs)` to make a software interrupt and when the thread is scheduled again, it runs our shell-code. 

-----------------------------------------------------------------------------------------------
code explanation:

-	‘buf’ contains the character array for shell-code. This shell-code (for demonstration purposes) is for a simple message box that says Hello World.
-	The way it works is, it creates a new svchost.exe process using the CreateProcessA() function from win API to create a process in the suspended state. 
-	Some memory is allocated in this suspended thread using VirtualAllocEx(). We cast the address of the remotebuffer (the virtually allocated memory) to a function pointer type LPTHREAD_START_ROUTINE then use WriteProcessMemory() to shellcode into the remote buffer. 
-	The VirtualProtectEX() function is what is changing this allocated memory from ReadWrite to ReadExecutable, and then we queue it to APC.


We are injecting the `Shell-code` into a well-know process like `svchost.exe` because, 
1.	Its harder to keep track of because there are many instances of svchost running in a windows computer
2.	The threads will be queued much faster if we use this (as it executes often)

Now everyone knows this, and hence such process are in fact monitored. But we can attach this to a thread in a process like `word.exe` and have our shell-code execute only when Microsoft word is launched. 

This is a better solution as it somewhat evades _sandbox detection_ as well, in case some blue team authors reach this. 
