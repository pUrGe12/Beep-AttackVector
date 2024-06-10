# Idea

Here, we’ll try to evade sandbox detection by keeping the malware dormant until a certain process is triggered. 

This process can be for example, notepad.exe. Inside a sandbox, no other processes such as notepad are opened other than the malware itself, but in a real user machine, we can identify certain applications that the user MUST open (it may even be a browser). 

Thus, we can identify if it’s a sandbox or not. 

We can do this by using a while true loop, creating a `snapshot` of all processes and iterating through them to see if our designated process is launched. 

In this example, I have used `notepad.exe` as my process and the program waits for notepad.exe to be launched before continuing further.

If we run this through `CFF Explorer`, we can see that all the process related functions are from `KERNEL32.DLL`. We can obfuscate the _IAT_ thereby making it harder for sandboxes to analyse which functions are being used.

![image](https://github.com/pUrGe12/Beep-AttackVector/assets/153343775/142234e7-3949-4a4e-b25c-e81c81fc3d51)

This is what happens inside a sandbox, the malware stays idle. 

Now security vendors approach this problem by **flagging the very use of the sleep function**. Then we can look at alternatives like the Beep function.

(Note that in order to run the malware samples in sandboxes, we need to use additional flags --> **-static -static-libgcc -static-libstdc++**)

But this is not enough. Good sandboxes `shorten sleep` times and make it look like the malware has slept for longer than it has. 

-	It can modify sleep cycles because sleep times are defined based on the CPU stats itself, and this is something that sandbox vendors have control over.

# Bypassing

To bypass that, we can use **beep** (because technically, the malware **does something** at this point, so not technically sleeping), or we can define a variable that increments along with the sleep cycles to cross verify that we’ve slept as we should have.

Now we need to look at ways to implement sleep functions without directly calling the sleep function -> one way is beep. This is because, these functions are easily recognisable by sandboxes,

![image](https://github.com/pUrGe12/Beep-AttackVector/assets/153343775/babe8f49-25bb-45f0-94a0-f759e1041601)

So, using Beep does in a way bypass this,

![image](https://github.com/pUrGe12/Beep-AttackVector/assets/153343775/1c151b64-ee6b-4576-88fd-9188fe16e010)

This is the code which achieves this for us. We keep looking for our specified process (Notepad.exe) and until that is present we make a beep sound every second. 

I am not sure if we can control the `volume of this beep` so as to not alert the user something’s happening. 


