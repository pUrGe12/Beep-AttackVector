# Code explanation

1.	We essentially perform two sets of analysis. One is to check if `SLL certificate` is present, and the other is to check the `HTML response` and validate that.
To perform the SSL check, I used this function called `InternetQueryOption`. What this does is, it has a lot of possible option flags and I ask it for a specific flag called `INTERNET_OPTION_SECURITY_FLAGS`. This retrieves an unsigned long integer value that contains the security flag for the handle we provide to `InternetQueryOption`. This option tells whether the connection is secured by SSL/TLS and other security attributes.

This is a very basic check, but from here we can try and validate the SSL certificate ourselves, by checking parameters such as **issued to, issued by and date till valid** (for which we can use `INTERNET_OPTION_SECURITY_CERTIFICATE`). We can do a deep dive from here, but if fake-net hasn’t been custom configured and uses default settings, then this also detects fake-net.

The SSLCheck function just checks if an SSL certificate is present, and if it is then it returns TRUE. Note that we haven’t exactly specified SSL here because that is done for the `InternetOpen()` function.

2.	We then check the HTML response. This check can also be made more robust, but for now, I am checking if the response contains tags like <html and <script. 

3.	Then I have the function called `IsConnectedToInternet()` and when this is called, it does the following things,

- It first establishes a handle for the use of `WinINET` functions. This is done using `InternetOpen()` and if this fails, then we exit the function (return FALSE).
- Then I make a buffer for 3 well-known URLs which I’ll be using to test the connection and security. 

The import function used here is,

![image](https://github.com/pUrGe12/Beep-AttackVector/assets/153343775/5625b2ea-478e-420e-b5f4-a53dfe58e77e)

For every URL we defined, we use the function InternetOpenUrl() with the handle we initialised earlier using InternetOpen(). The import thing to note is the use of 

    INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_SECURE

-	`INTERNET_FLAG_SECURE` tells the function to use secure protocols like SSL/TLS to connect to the URL.
-	`INTERNET_FLAG_NO_CACHE_WRITE` tells the function to not write any data in the cache memory. This is important because if we don’t want to leave any traces.

Once we get this handle, we run the `SSLCheck function` defined before along with the HTML code check function.

-	Use the `InternetReadFile()` function which is used to read data from a handle opened by `InternetOpenUrl()` and returns TRUE if it gets data. We receive all the data we are getting after trying to establish a connection with above URLs. The hConnect handle is passed to this and we receive all the data and then null terminate it.
-	Then we call the `CheckContent` function to validate the HTML code.

Only if both return true, the function `IsConnectedToInternet()` returns TRUE. Finally, all this allows us to verify if a sandbox is using fakeNet and then if it is, then not establish connections to a C2 server. 

(It won’t run on a sandbox unless we add `-static-libstdc++` while compiling it because the sandboxes for some reason don’t have this installed and statically loaded).

Note that, not all sandboxes use FakeNet so this test alone won’t suffice.
