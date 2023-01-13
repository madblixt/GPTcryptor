# GPTcryptor

To use this program, you will need to compile it using a C++ compiler.

You can use the following command to compile the program in the command line:

Copy code
`g++ -std=c++11 -lssl -lcrypto encrypt_executable.cpp -o encrypt_executable`
You can then run the program by providing the path to an executable file as the first argument:

Copy code
`./encrypt_executable /path/to/executable [output_file]`
The output file is optional, if not provided the original file will be overwritten.

After running the key and iv generator program, you will have to copy the key and iv from the output and paste them in the encryption program.

You can replace the key and iv in the main script like this:

`unsigned char key[] = {...}; // 32 bytes`
`unsigned char iv[] = {...}; // 16 bytes`

with the key and iv from the output of the generator program, for example:

```unsigned char key[] = {0x5b, 0x37, 0x6e, 0x27, 0x89, 0x5d, 0x3f, 0x68, 0x2b, 0x7c, 0x5e, 0x3a, 0x6f, 0x2d, 0x7b, 0x50,... 0x7e};```

```unsigned char iv[] = {0x5b, 0x37, 0x6e, 0x27, 0x89, 0x5d, 0x3f, 0x68, 0x2b, 0x7c, 0x5e, 0x3a, 0x6f, 0x2d, 0x7b};```
