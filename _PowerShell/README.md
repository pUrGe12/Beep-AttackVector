# Decrypt, save, compile and execute  

Note that, we are running with the assumption that g++ is installed in the victim’s computer (an appropriate assumption if the victim is a computer scientist). 

We start with initialing a data variable to the value obtained from Base64 decoding the given string. Understand that, during AES encryption, the final output can either be in _hex format_ or _base64 encoded_. In this case, we have taken the base64 output and hence, we first get the raw AES encrypted data. 


Code explanation
--------------------------------------------------------------------------------------------------------------------------------------------------------------

We start with the Convert class and a data variable,
```
[Convert] is a .NET class that provides methods and functions for interchanging data types. In this case, we have used it to get the function FromBase64String(). 
$data is the variable that stores the decoded string
```

The `Initialisation Vector` (IV) and the `key` is defined and these are the same values used to encrypt the C++ program. The key must be padded with null character because it needs to be exactly 16 bytes long. In our case however, the key has been predefined to be 16 bytes long and hence, padding is not really necessary. 

Then we create the `AES decryptor`, 

-	We start with the `System.Security.Cryptography.Aes` class in .NET. This is part of the _System.Security.Cryptography_ namespace which provides cryptographic algorithms. We use this class and `create()` an instance of it. This is a static method and it’s called on the class itself rather than on an instance of the class. 

-	We use the namespace `System.Text` which contains various character encodings like _ASCII and Unicode_. The Encoding class is used to convert one encoding to another. We access the `utf8 method` of this class and store that for converting any string to utf8 byte arrays.

-	We then get the bytes from the key and the bytes from the IV and utf8 encode it. We set the `byte array` as the AES key, and the AES IV as the utf8 encoded IV-byte array. 

-	The function `CreateDecryptor()` is present in the AES class and is used to decrypt AES encrypted strings. We create an instance of the decryptor as `dec`.

-	Using the decryptor, we call the `TransformFinalBlock()` function which takes in the raw AES data (after decoding it). We define the positions from which it has to decrypt (0 to the final length). Note that the decryptor already has information of the IV and the key as we set it before. We store the decrypted string in the result variable.

-	Then we encode this byte-array using _UTF-8_ to make it make sense.

Finally, the decrypted string is written to a file which is compiled using g++. 

The final program is executed after compilation. 

To AES encrypt the C++ code (which will be described later) I used this [link](https://www.devglan.com/online-tools/aes-encryption-decryption). It allows us to set the output format, IV and key as per our wish. 
