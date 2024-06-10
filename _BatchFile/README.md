The thing with windows PowerShell is that, a user is not allowed to run PS scripts without setting the `execution policy to Unrestricted or Bypass`. 

The default execution policy in PowerShell is restricted and this is because PowerShell can directly interfere with the system OS and hence is dangerous (in fact, it loads the `AMSI.dll` which is an additional security implantation from where this feature probbaly comes from).

Only the `Administrator` has the power to change the execution policy. 

Since, we cannot run the PS script we developed above without changing the execution policy, we change the _execution policy for only executing this script_. This is done using the CMD script,

-	The` @echo off` command is used in batch scripting to disable the display of command lines in the command prompt window. The `@` ensures that this particular command is also not shown in the command prompt. This is important because we are trying to stay hidden here
-	We run the PowerShell command by creating a new instance of PS, to change the `ExecutionPolicy to Bypass`. We then use the `-File` argument to run the given PS file.

This is an effective way to bypass the use of admin privileges to run PS commands. I have no idea why this works.

---------------------------------------------------------------------------------------------------------------------------------------------------
The batch script is injected into an `autorun` registry, and everytime it is executed, it runs the powershell script. 
