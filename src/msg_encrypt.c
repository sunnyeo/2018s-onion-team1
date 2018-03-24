#include <stdio.h>
#include <stdlib.h>

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>

#include <arpa/inet.h> /* For htonl() */


/*
root@ubuntu:~/tmp# 
root@ubuntu:~/tmp# openssl genrsa -out key.pem
Generating RSA private key, 2048 bit long modulus
..........................+++
.............+++
e is 65537 (0x10001)
root@ubuntu:~/tmp# openssl rsa -in key.pem -out key.pub -pubout
writing RSA key
root@ubuntu:~/tmp# ls
key.pem  key.pub
root@ubuntu:~/tmp# echo 'test...' > plaintext
root@ubuntu:~/tmp# openssl rsautl -in plaintext -out rsaencrypted -pubin -inkey key.pub -encrypt
root@ubuntu:~/tmp# ls
key.pem  key.pub  plaintext  rsaencrypted
root@ubuntu:~/tmp# cat rsaencrypted 
p�R:�݉��΀Cؼ��  �
                .bmR� �S! ��2 9�
9 i                             ��<f� ��Ů�:�E � �Fk�,��� y$  �c��8�PU�����2�G��J��b[�� �_�]���J �d�  �+� ̲86�@���
   �h�.- W]�y\���JQ�g��̛�2���1� �=��v�q��d{ ��e �
                                                ��y��� ?  ��c}��i,S` ״  ;�����a�*: �E1�2��
                                                                                          mi Ɏ9�߁ �F�����1i;�

                                                                                                             ��  vwroot@ubuntu:~/tmp# 
root@ubuntu:~/tmp# 
root@ubuntu:~/tmp# openssl rsautl -in rsaencrypted -out rsadecrypted -inkey key.pem -decrypt
root@ubuntu:~/tmp# cat rsadecrypted 
test...
root@ubuntu:~/tmp

*/

int file_encrypt(char *filename){
	
}


/*
첫번째시도
Laura$ gpg --import hansh17.pub 
gpg: key 6DBC89AE: public key "Seongho Han (SH) <hansh09@kaist.ac.kr>" imported
gpg: Total number processed: 1
gpg:               imported: 1  (RSA: 1)
gpg: 3 marginal(s) needed, 1 complete(s) needed, PGP trust model
gpg: depth: 0  valid:   1  signed:   0  trust: 0-, 0q, 0n, 0m, 0f, 1u
Laura$ 

두번째시도
Laura$ gpg --import hansh17.pub 
gpg: key 6DBC89AE: "Seongho Han (SH) <hansh09@kaist.ac.kr>" not changed
gpg: Total number processed: 1
gpg:              unchanged: 1

"gpg: key" 문자열 다음으로 파싱해오면될것같은데...
*/

// ex) 로컬에 hansh17.pub 파일이 있는 상태에서, publickey 문자열(6DBC89AE)을 리턴하는 함수
char get_key(char *githubID){
	char *publickey;
	// [TODO] 
	return publickey;
}




int do_evp_seal(FILE *rsa_pkey_file, FILE *in_file, FILE *out_file)
{
    int retval = 0;
    RSA *rsa_pkey = NULL;
    EVP_PKEY *pkey = EVP_PKEY_new();
    EVP_CIPHER_CTX ctx;
    unsigned char buffer[4096];
    unsigned char buffer_out[4096 + EVP_MAX_IV_LENGTH];
    size_t len;
    int len_out;
    unsigned char *ek = NULL;
    int eklen;
    uint32_t eklen_n;
    unsigned char iv[EVP_MAX_IV_LENGTH];

    if (!PEM_read_RSA_PUBKEY(rsa_pkey_file, &rsa_pkey, NULL, NULL))
    {
        fprintf(stderr, "Error loading RSA Public Key File.\n");
        ERR_print_errors_fp(stderr);
        retval = 2;
        goto out;
    }

    if (!EVP_PKEY_assign_RSA(pkey, rsa_pkey))
    {
        fprintf(stderr, "EVP_PKEY_assign_RSA: failed.\n");
        retval = 3;
        goto out;
    }

    EVP_CIPHER_CTX_init(&ctx);
    ek = malloc(EVP_PKEY_size(pkey));

    if (!EVP_SealInit(&ctx, EVP_aes_128_cbc(), &ek, &eklen, iv, &pkey, 1))
    {
        fprintf(stderr, "EVP_SealInit: failed.\n");
        retval = 3;
        goto out_free;
    }

    /* First we write out the encrypted key length, then the encrypted key,
     * then the iv (the IV length is fixed by the cipher we have chosen).
     */

    eklen_n = htonl(eklen);
    if (fwrite(&eklen_n, sizeof eklen_n, 1, out_file) != 1)
    {
        perror("output file");
        retval = 5;
        goto out_free;
    }
    if (fwrite(ek, eklen, 1, out_file) != 1)
    {
        perror("output file");
        retval = 5;
        goto out_free;
    }
    if (fwrite(iv, EVP_CIPHER_iv_length(EVP_aes_128_cbc()), 1, out_file) != 1)
    {
        perror("output file");
        retval = 5;
        goto out_free;
    }

    /* Now we process the input file and write the encrypted data to the
     * output file. */

    while ((len = fread(buffer, 1, sizeof buffer, in_file)) > 0)
    {
        if (!EVP_SealUpdate(&ctx, buffer_out, &len_out, buffer, len))
        {
            fprintf(stderr, "EVP_SealUpdate: failed.\n");
            retval = 3;
            goto out_free;
        }

        if (fwrite(buffer_out, len_out, 1, out_file) != 1)
        {
            perror("output file");
            retval = 5;
            goto out_free;
        }
    }

    if (ferror(in_file))
    {
        perror("input file");
        retval = 4;
        goto out_free;
    }

    if (!EVP_SealFinal(&ctx, buffer_out, &len_out))
    {
        fprintf(stderr, "EVP_SealFinal: failed.\n");
        retval = 3;
        goto out_free;
    }

    if (fwrite(buffer_out, len_out, 1, out_file) != 1)
    {
        perror("output file");
        retval = 5;
        goto out_free;
    }

    out_free:
    EVP_PKEY_free(pkey);
    free(ek);

    out:
    return retval;
}

int main(int argc, char *argv[])
{
    FILE *rsa_pkey_file;
    int rv;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <PEM RSA Public Key File>\n", argv[0]);
        exit(1);
    }

    rsa_pkey_file = fopen(argv[1], "rb");
    if (!rsa_pkey_file)
    {
        perror(argv[1]);
        fprintf(stderr, "Error loading PEM RSA Public Key File.\n");
        exit(2);
    }

    rv = do_evp_seal(rsa_pkey_file, stdin, stdout);

    fclose(rsa_pkey_file);
    return rv;
}