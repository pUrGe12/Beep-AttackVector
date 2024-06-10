## Explanation

The loader binary uses a range of **anti-sandbox and anti-debugging** techniques,

-	FakeNet detection
-	INT 3 check – Anti-debug
-	RDTSC check 
-	Delayed execution

If any check fails, the loader binary will quickly terminate any further execution. If all the checks pass, it does the following,

```
Load the Big.dll file and import its functions.
Create the batch file and the PS script and save them.
Launch the batch file in the auto-start system registry to have it launch whenever the system reboots.
```

----------------------------------------------------------------------------------------------------------
## Getting the PS and batch script,

We call the functions `scriptA()` and `batscriptA()` to get the PS script and batch script respectively. 

Then we declare an `unordered_map` to store the frequency table used in creating the Huffman codes. The keys are characters, and the values are their corresponding Huffman codes (as strings).

```
An unordered_map is an associated container that stores elements formed by the combination of a key value and a mapped value.
The key value is used to uniquely identify the element and the mapped value is the content associated with the key.
The unordered_map is like a data structure of dictionary type that stores elements in itself.
It contains successive pairs (key, value), which allows fast retrieval of an individual element based on its unique key.
Internally unordered_map is implemented using Hash Table, the key provided to map is hashed into indices of a hash table which is why the performance of data structure depends on the hash function a lot but on average, the cost of search, insert, and delete from the hash table is O(1).
We use it here because it allows for an efficient lookup. 
```

-	We then declare a `shared pointer` to the root of the Huffman tree. The Node type represents a node in the Huffman tree, and this structure has been defined before (refer to the understanding Huffman codes section).

-	We then build the Huffman Tree using the script we get from `scriptA()`, the unordered_map and the Node pointer. Then using this, we call the `decodeHuffman()` function, where `HuffmanStr` stores the Huffman tree in a nodal string structure.

Then using ofstream (from **iostream**) we write two files, the PS script and the batch script.

----------------------------------------------------------------------------------------------------------
## registry launching

Now we launch the batch script into an auto-run registry, 

![image](https://github.com/pUrGe12/Beep-AttackVector/assets/153343775/08f4f273-8d67-47ca-af4f-fc7c927c4b4d)

Here the batch file is deployed inside the `HKCU\SOFTWARE\Microsoft\Windows\CurrentVersion\Run` registry key. 

What this means is, the binary file opens this registry key, writes the executable (or `BSdecoded.bat` in this case) to the registry key, and then closes it. 

This registry key is launched (that is, its contents are run) every time the system reboots. Using the `RegOpenKeyEx()` function I opened the specified registry key. The `KEY_WRITE` variable checks if the specified registry has writable permissions, and if it does then it returns ERROR_SUCCESS. 

Then I used `RegSetValueEx()` to write the batch file to the registry and then close it.
