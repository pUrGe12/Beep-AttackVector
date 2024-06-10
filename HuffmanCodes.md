# The node structure

![image](https://github.com/pUrGe12/Beep-AttackVector/assets/153343775/e14e46ab-75d8-426e-9aec-f1c484515117)

We have first defined a node structure. This structure contains the character that the node represents (note that only the leaf of the tree --> the two nodes at the bottom of the tree, have a character. 

For others, this value is set to `\0`). It contains the frequency of the character that the node represents --> for leaf nodes it is the number of times these have appeared and for other nodes, it’s the sum of the frequencies of all characters in the subtree rooted at this node. 

Finally, it contains a “pointer” to the left and right of the node. 

Then we have defined constructors for the leaf node and the internal nodes. For the `leaf node`, we initialise it with a given character ‘c’ and frequency ‘f’. The left and right pointers are set to ‘\0’ because leaf nodes do not have children (they are the last ones). Then we have the internal node and we initialise it with the given character ‘c’ (often '\0' for internal nodes), frequency ‘f’, and pointers to the left and right child nodes ‘l’ and ‘r’.

# The compare structure (/operator)

![image](https://github.com/pUrGe12/Beep-AttackVector/assets/153343775/3d9a8ebb-d6ea-4809-bbf4-10dc831cb8d2)

This is an example of an `overloaded operator` (also called the call operator). It allows instances of the Compare structure to be used as if they were functions. 

This is a common pattern in C++ for defining `custom comparison logic`, especially for use in standard library containers like `std::priority_queue` (which is where we are using it).

The function returns true if the frequency of the node pointed to by ‘l’ is greater than the frequency of the node pointed to by ‘r’. Otherwise, it returns false.

This comparison logic ensures that the `priority queue behaves as a min-heap`. In a min-heap, the smallest element is always at the top (exactly what we are looking for). 

By returning true when l->freq is greater than r->freq, we tell the priority queue to place nodes with smaller frequencies higher in the queue.


# Building the Huffman Tree

![image](https://github.com/pUrGe12/Beep-AttackVector/assets/153343775/a4fb8670-fb51-4f6d-892d-c77452d3fdc7)

Here the text variable is used as the input string for which we want to build the Huffman Tree, the `huffmanCode` variable is an unordered_map that will store the frequency table, and the root variable is a shared pointer to the root node of the Huffman Tree. 

-	First we calculate the frequency of each character by calling a for loop to go over the text and increment the count of that character in the unordered_map which we have initialised as freq.

-	Then we create a priority queue to store the nodes of the Huffman Tree prioritised by its frequency. It essentially keeps the order sorted for our needs. We use the compare class to basically compare different nodes based on frequency.
 
The shared_ptr is a smart pointer that manages the lifetime of dynamically allocated objects. The expression, `shared_ptr<Node>` specifies the type of element being stored in the priority queue (we store them as vectors).

The final handle is passed to the variable called pq which we will make use of later.

-	Then we iterate over each element in the frequency table (Huffman table) and push a shared_ptr to the new node into the queue. The make_shared function allocates and constructs an object of the Node type and returns a shared_ptr to it. We create this using the current element in the freq map and the frequency of the character. 

-	Then we continue the loop as long as the priority queue has only 1 node. Inside the loop we extract the node with the lowest frequency, (left) and then the next node with the lowest priority (right).
  
-	Then we create a new internal node with a frequency equal to the sum of the left and right’s frequencies. This node has the two extracted nodes as its children. Then we **push this new node back onto the priority queue**.

We continue this until there is 1 node left, this is the root of the Huffman tree.

-	Then I’ve set the root of the Huffman tree as the top element in the priority queue.

### Finally, we define a function to generate the Huffman Codes from this tree.

![image](https://github.com/pUrGe12/Beep-AttackVector/assets/153343775/b91bae00-ff4c-479f-a829-af60b0259b26)

This is a `lambda function` (an in-line function) called `encode` which takes in a node (a shared_ptr to the node) and a string as inputs. 

It is called `recursively` and it assigns codes to each character present in the string it is going to run over. 

The use of `[&]` in the code is for a capture clause that captures all local variables in the enclosing scope by reference. This allows the lambda to access and modify the frequency map, and strings defined outside the lambda function.

-	If the current node is a null_ptr `(\0)` then this indicates that we have reached the end of the loop and we break out. Else, we check if the current node is a leaf node. This means that it does not have a left or right node. (note that, leaf nodes means nodes that contain characters, while internal nodes are summations of leaf nodes and not actual characters).

-	If current node is a leaf node, we assign the value of ‘str’ to the ‘ch’ key of the node inside the frequency table. The key is the character, and the value is its corresponding Huffman code. 

-	We then make recursive calls to the `left child appending “0”` to the current Huffman string (cause, if we go left, it means an extra 0) and the right child, appending “1”.

-	Then finally, we initiate the process, by calling the encode function on the root node and an empty initial value. The values are filled depending on whether it goes left or right and then this value is assigned to the character inside the frequency map.

Thus, the `buildHuffmanTree(`) modifies the frequency table (or the Huffman tree). This is used by the decoding function to decode an encoded string.

# Decoding the Huffman string

This function will take in an encoded Huffman string (rather, a reference to it, because you don’t want to accidently modify it and make it bad) and a pointer to the root of the Huffman tree. 

![image](https://github.com/pUrGe12/Beep-AttackVector/assets/153343775/d589533e-b983-43a0-bb10-484e17b037d7)

-	We define the function to take in a reference to the encoded string and a shared pointer to the root of the `Huffman tree`. In the next couple lines, the result variable and root are initialised.

-	Then there is a loop that goes over each character in the encoded string (note that this is a bunch of 0s and 1s), and if the bit is `0`, we move to the left child of the current node and if its `1` then we move to the right child.

 	We continue this, until there are no left or right children, at which point, we must have reached the leaf node, and therefore we need this character in the result string.

-	Then we reset the current node to the root node and start over again. Once the loop is completed, the function returns the result.

These functions are used by the loader to decode the Huffman string.

To encode my PowerShell script, I have used another program I made, which will take in the PowerShell script and return the encoded version of it. The working of this is the complete opposite to the decoding script above with the exception of generating the Huffman Tree.

