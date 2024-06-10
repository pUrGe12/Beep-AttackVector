$data=[Convert]::FromBase64String('<base64_AES_encrpyted>');
$iv='abcdefghijklmnop';
$key='q3By2ZFhFis1iZWe'.PadRight(16, [char]0);
$aes=[System.Security.Cryptography.Aes]::Create();
$utf8=[System.Text.Encoding]::Utf8;
$aes.Key=$utf8.GetBytes($key);
$aes.IV=$utf8.GetBytes($iv);
$dec=$aes.CreateDecryptor();
$result =$dec.TransformFinalBlock($data,0,$data.Length);
$resultStr=$utf8.GetString($result);
$filePath='C:\\path\\to\\file.cpp';
sSet-Content -Path $filePath -Value $resultStr;
$command= 'g++ -o test C:\\path\\to\\file.cpp';
Invoke-Expression $command;$command2 = '.\\test.exe';
Invoke-Expression $command2;
$dec.Dispose()
