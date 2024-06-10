This contains functions to transfer the CMD script, the PowerShell script and functions to decode the Huffman codes. It must be compiled using the following,

    g++ -shared -o Big.dll <filename.cpp>

The functions exported are:

- buildHuffmanTree()
- decodeHuffman()
- scriptA() --> to get the plaintext PowerShell script
- batscriptA() --> to get the plaintext batch script

We export these functions using the extern command, 
![image](https://github.com/pUrGe12/Beep-AttackVector/assets/153343775/efc6f57a-3882-4ca5-b9aa-369007e2c3b5)

This allows such functions to be used by any program that loads this DLL. Note that, we have not defined an API entry point for this DLL. 

The imported libraries are:
-	iostream					    -- 		to save the scripts into a file
-	unordered_maps				-- 		used in Huffman codes as described
-	vector					      -- 		to use the vector data-type
-	queue					        -- 		to use queues
-	string					      -- 		perform some string manipulation commands
-	memory					      -- 		accessing 
-	windows				        -- 		using windows API functions

The descriptions of `buildHuffmanTree()` and `decodeHuffman()` are given in the Huffman codes section.
