Here we are trying to figure out debuggers by an analysis of breakpoints:

When a breakpoint is set, there is a special code `0xCC` which is placed at a specific address in the code. This is the `INT 3` instruction, which is an **interrupt** used specifically for debugging purposes. This is the standard method used by most debuggers for x86 architectures. To check if a debugger is placed in the code, we can do the following,
```
The function scan’s its own memory for the opcode
```

GDB does not remove "standard" breakpoints one they are hit. Temporary breakpoints (inserted with tbreak) are permanently removed once hit, though. GDB sets breakpoint after function prologue, because function prologue is not typically what gdb users want to see. So, if you put break to foo, actual breakpoint will be typically located `few bytes after that` (depends on prologue code itself that is function dependent as it may or might not have to save stack pointer, frame pointer and so on).

So, when a breakpoint has been set in the main function, the opcode will be placed somewhere in the memory. WHat we can do is, go over each of the 256 bytes and see if it `0xCC` and if it is then it means that a debugger is analysing it.

