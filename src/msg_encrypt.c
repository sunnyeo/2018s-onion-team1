#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>

#include <arpa/inet.h> /* For htonl() */
#define BUFF_SIZE 1024


int msgfile_encrypt(char *file_name, char *githubId){
    char cmd[256];
	char* keyID;
	if(!to_exist_publickey(githubId)) {
		printf("error! cannot get %s.pub...\n",githubId); 
		exit(202);
	}
	keyID = get_pubkeyID(githubId);
	
	snprintf(cmd, 256,"echo keyid is %s >> log",keyID); system(cmd);
    snprintf(cmd, 256, "gpg --trust-model always --armor --encrypt --recipient %s %s 2>/dev/null", keyID, file_name); system(cmd);
	snprintf(cmd, 256, "mv %s.asc %s", file_name, file_name); system(cmd); 
    return 0;
}

// HERE
int msgfile_decrypt(char *filename, char *passphrase){
	char cmd[256];
	// 아 디크립트가 안되니까 tmp파일이 안생기는건가??? 디크립트 실패하나봐... 왜냐면 암호화자체가 안되있기 때문이지... [HERE]
	snprintf(cmd, 256, "gpg --batch --output %s.tmp --passphrase %s --decrypt %s 2>/dev/null", filename, passphrase, filename); //output : filename.tmp
	system(cmd);
	snprintf(cmd, 256, "mv %s.tmp %s", filename, filename);
	system(cmd);
	return 0;

}

int msgfile_sign(char *filename, char *passphrase){ 
	char cmd[256];
	snprintf(cmd, 256, "gpg --passphrase %s --sign %s 2>/dev/null", passphrase, filename);
	system(cmd);
	snprintf(cmd, 256, "mv %s.gpg %s", filename, filename);
	system(cmd);
// encryption과 함께 사용하고 싶으면 gpg --armor -o [output.txt] --passphrase [passphrase] --sign --encrypt -r [recipient's ID] [input.txt] 를 사용한다. 
} 

// decrypt and verify
int msgfile_sign_verify(char *filename, char *githubId, char *passphrase){
	char cmd[256];
	char cmd_temp[256];
	char cmd_result[256];
	char key_temp[30];
	char *temp;
	char *key;
	FILE *import;

	snprintf(cmd_temp, 100, "gpg --import %s.pub 2>&1", githubId);
	import = popen(cmd_temp, "r");
	
	if(import == NULL){
		printf("There is no %s.pub file\n", githubId);
		return -1;
	}

	fgets(cmd_result, 30, import);
	temp = strstr(cmd_result, "key");
	key = strstr(temp, " ");
	key = strncpy(key_temp, key+1, 15);
	pclose(import);

	snprintf(cmd, BUFF_SIZE, "gpg --passphrase %s --decrypt %s > %s.tmp", passphrase, filename, filename);
	system(cmd);
	snprintf(cmd, BUFF_SIZE, "mv %s.tmp %s", filename, filename);
	system(cmd);
	
	if(fsize(filename)==0) return 0;
	else return 1;
}

