#include <iostream>
#include <fstream>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <vector>

std::string readFile (const char *fileName)
{
  std::ifstream file (fileName, std::ios::binary | std::ios::ate);
  std::streamsize size = file.tellg ();
  file.seekg (0, std::ios::beg);
  std::string buffer (size, 0);
  if (file.read (buffer.data (), size))
    {
      return buffer;
    }
  return "";
}

void
writeFile (const char *fileName, const std::string & data)
{
  std::ofstream file (fileName, std::ios::binary);
  file.write (data.c_str (), data.size ());
}

void
encryptAES256 (std::vector < unsigned char >&data, unsigned char *key,
	       unsigned char *iv)
{
  EVP_CIPHER_CTX *ctx;
  int len;
  int ciphertext_len;
  ctx = EVP_CIPHER_CTX_new ();
  EVP_EncryptInit_ex (ctx, EVP_aes_256_cbc (), NULL, key, iv);
  int data_size = data.size ();
  EVP_EncryptUpdate (ctx, data.data (), &len, data.data (), data_size);
  ciphertext_len = len;
  EVP_EncryptFinal_ex (ctx, data.data () + len, &len);
  ciphertext_len += len;
  EVP_CIPHER_CTX_free (ctx);
  data.resize (ciphertext_len);
}

void
decryptAES256 (std::vector < unsigned char >&data, unsigned char *key,
	       unsigned char *iv)
{
  EVP_CIPHER_CTX *ctx;
  int len;
  int plaintext_len;
  ctx = EVP_CIPHER_CTX_new ();
  EVP_DecryptInit_ex (ctx, EVP_aes_256_cbc (), NULL, key, iv);
  int data_size = data.size ();
  EVP_DecryptUpdate (ctx, data.data (), &len, data.data (), data_size);
  plaintext_len = len;
  EVP_DecryptFinal_ex (ctx, data.data () + len, &len);
  plaintext_len += len;
  EVP_CIPHER_CTX_free (ctx);
  data.resize (plaintext_len);
}

int
main (int argc, char *argv[])
{
  if (argc < 2)
    {
      std::cout << "Usage: encrypt_executable <executable> [output]" << std::
	endl;
      return -1;
    }

  // Read the executable file
  std::string data = readFile (argv[1]);
  if (data.empty ())
    {
      std::cout << "Error: unable to read file" << std::endl;
      return -1;
    }

// Convert data to a vector for easy resizing
  std::vector < unsigned char >data_vec (data.begin (), data.end ());

// Encrypt the data
  unsigned char key[] = { ... };	// 32 bytes
  unsigned char iv[] = { ... };	// 16 bytes
  encryptAES256 (data_vec, key, iv);

// Add decryption routine to the beginning of the data
  std::vector < unsigned char >decryptionRoutine;
  decryptionRoutine.insert (decryptionRoutine.end (),
			    reinterpret_cast <
			    unsigned char *>(decryptAES256),
			    reinterpret_cast <
			    unsigned char *>(decryptAES256) +
			    reinterpret_cast <
			    unsigned char *>(main) - reinterpret_cast <
			    unsigned char *>(decryptAES256));
  data_vec.insert (data_vec.begin (), decryptionRoutine.begin (),
		   decryptionRoutine.end ());

// Modify the entry point to point to the decryption routine
  const int offset = decryptionRoutine.size ();
  *reinterpret_cast < int *>(&data_vec[0x18]) += offset;

// Write the output file
  const char *outputFile = (argc > 2) ? argv[2] : argv[1];
  writeFile (outputFile, std::string (data_vec.begin (), data_vec.end ()));

  std::cout << "Encryption successful. Output file: " << outputFile << std::
    endl;
  return 0;
}
