# Beep

## The actual working

Beep is an information stealer malware that uses three separate components: a dropper, an injector, and the payload.

The dropper `big.dll` creates a new registry key with an `AphroniaHaimavati` value (just a name) that contains a base64 encoded PowerShell script. This PowerShell script is launched every 13 minutes using a Windows scheduled task.

When the script is executed, it will download data and save it to an injector named `AphroniaHaimavati.dll`, which is then launched.

The injector is the component that uses a range of anti-debugging and anti-VM techniques to inject the payload into a legitimate system process like `WWAHost.exe` via `process hollowing` to evade detection from anti-virus tools running on the host.

Finally, the primary payload attempts to collect data from the compromised machine, encrypt it, and send it to the C2 (establish a backdoor).

## This attack vector

The attack sequence comprises of decompression and decryption sequences to finally reveal a `backdoor shellcode` that is injected into a process. Note that the shellcode I have used is for a simple **message box at the moment**. This malware uses anti-sandboxing and anti-debugging techniques to remain undetected. The files that comprise of the entire attack sequence are:

```
Load.exe
Big.dll
```

The files created during runtime are: 

```
BSdecoded.dat
PSscriptDecoded.ps1
Test.exe
```

The overall flow of the attack sequence

- When the `load.exe` binary is executed, it calls the DLL, _Big.dll_. It contains _anti-debug and anti-sandbox_ techniques that will be discussed later in the document.
- The load.exe binary contains a `Huffman encoded PowerShell` script. The binary uses functions imported from the DLL to decode this and save it into a file named `PSscriptDecoded.ps1`
- It also calls another function from the DLL to load a batch script (.bat script) and save that into `BSdecoded.dat`, and take the original PowerShell (not encoded) because it is to be used in creating the frequency table.

Alternatively, we can directly import the _frequency table_ for the script as that would make the data better obfuscated. We can also choose to use the `base64 encoded` versions of the CMD script and the original PS and decode them during runtime.

-	It then injects the BSdecoded.dat into an `autorun registry` and terminates itself. 
-	When the system is rebooted, the _BSdecoded.dat_ file executes, which in turn runs _PSscriptDecoded.ps1_. 
-	The PowerShell script contains `AES 128-bit CBC` mode encrypted C++ code, which it decodes once executed, compile that into an executable file (`test.exe`) and run that. After that, it can be made to destroy the test.exe file.
-	The final payload lies in the test.exe binary which is a shell-code for a backdoor (ideally, but in this case, it’s just a message-box) and the binary injects this shell-code into a suspended thread and queues it.

