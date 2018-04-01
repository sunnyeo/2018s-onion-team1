#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>

#include <arpa/inet.h> /* For htonl() */
#define BUFF_SIZE 1024

char *get_pubkeyID(char *githubId); // 원형선언을 안해놓으면 지멋대로 왜 함수 리턴타입을 integer 로 게싱을 하지?

int msgfile_encrypt(char *file_name, char *githubId){
    char cmd[256];
	char* keyID;
	if(!to_exist_publickey(githubId)) {
		printf("error! cannot get %s.pub...\n",githubId); 
		exit(202);
	}
	keyID = get_pubkeyID(githubId);
    snprintf(cmd, 256, "gpg --trust-model always --armor --encrypt --recipient %s %s 2>/dev/null", keyID, file_name); system(cmd);
	snprintf(cmd, 256, "mv %s.asc %s", file_name, file_name); system(cmd); 
    return 0;
}

int msgfile_decrypt(char *filename, char *passphrase){
	char cmd[256];
	snprintf(cmd, 256, "gpg --batch --output %s.tmp --passphrase %s --decrypt %s 2>/dev/null", filename, passphrase, filename); //output : filename.tmp
	system(cmd);
	snprintf(cmd, 256, "mv %s.tmp %s", filename, filename);
	system(cmd);
	return 0;

}

int msgfile_sign(char *filename, char *passphrase){ 
	char cmd[256];
	
	snprintf(cmd, 256, "gpg --batch --trust-model always --armor --output %s.sig --passphrase %s --sign onion 2>/dev/null", filename, passphrase); system(cmd);
	snprintf(cmd, 256, "mv %s.sig %s", filename, filename); system(cmd);
// encryption과 함께 사용하고 싶으면 gpg --armor -o [output.txt] --passphrase [passphrase] --sign --encrypt -r [recipient's ID] [input.txt] 를 사용한다. 
} 

// decrypt and verify
int msgfile_sign_verify(char *filename, char *githubId, char *passphrase){
	char cmd[256];
	char buf[256];
	char *pkey = NULL;
	char *skey = NULL;
	FILE *fp;
	pkey = get_pubkeyID(githubId);
	
	if(!pkey) return 0; // cannot get keyid
	
	// get keyid of sign
	snprintf(cmd, 256, "gpg --verify %s 2> signature.tmp", filename); system(cmd);
	
	fp = fopen("signature.tmp", "r"); //read write and create
	
	while(fgets(buf, 256, fp)){
		skey = strstr(buf, "key ID ");
		if(skey) break;
	}
	fclose(fp);
	if(!skey) return 0; // file isn't signed
	
	skey += strlen("key ID ");
	
	if(!strncmp(pkey,skey,8)){ // sign verified
		snprintf(cmd, 256, "gpg --output %s.tmp --decrypt %s 2>/dev/null", filename, filename); system(cmd);
		snprintf(cmd, 256, "sudo mv %s.tmp %s", filename, filename); system(cmd);
		system("sudo rm signature.tmp");
		return 1;
	}
	else {
		system("sudo rm signature.tmp");
		return 0;
	}
}
