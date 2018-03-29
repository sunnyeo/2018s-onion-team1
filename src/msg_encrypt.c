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
    
    char cmd[100];
    char cmd_result[30];
    char key_temp[30];
    char *temp;
    char *key;
    FILE *import;
	to_exist_publickey(githubId);
    snprintf(cmd, 100, "gpg --import %s.pub 2>&1", githubId);
    import = popen(cmd, "r");
    if(import == NULL){
        printf("There is no %s.pub file\n", githubId);
        return -1;
    }
    if(!fgets(cmd_result, 29, import)) { 
        printf("There is no %s.pub file\n", githubId);
        return -1;
	}
    temp = strstr(cmd_result, "key");
    key = strstr(temp," ");
    key=strncpy(key_temp, key+1, 15);
    pclose(import);

    snprintf(cmd, 100, "gpg --trust-model always --armor --encrypt --recipient %s %s 2>/dev/null", key, file_name); 
    system(cmd);
	snprintf(cmd, 100, "mv %s.asc %s", file_name, file_name); 
	system(cmd);

    return 0;
}
// 인크립트호출해도 로컬에 onion이 그대로 남아있고
// 디크립트만따로호출해서 디크립트해보면 실패하는게
// 아..음..
int msgfile_decrypt(char *filename, char *passphrase){
	char cmd[BUFF_SIZE];
	snprintf(cmd, BUFF_SIZE, "gpg --passphrase %s --decrypt %s > %s.tmp 2> /dev/null", passphrase, filename, filename); 
	system(cmd);
	snprintf(cmd, BUFF_SIZE, "mv %s.tmp %s", filename, filename);
	system(cmd);
	return 0;

}

int msgfile_sign(char *filename, char *passphrase){ 
	char cmd[BUFF_SIZE];
	snprintf(cmd, BUFF_SIZE, "gpg --passphrase %s --sign %s 2>/dev/null", passphrase, filename);
	system(cmd);
	snprintf(cmd, BUFF_SIZE, "mv %s.gpg %s", filename, filename);
	system(cmd);
// encryption과 함께 사용하고 싶으면 gpg --armor -o [output.txt] --passphrase [passphrase] --sign --encrypt -r [recipient's ID] [input.txt] 를 사용한다. 
} 

// decrypt and verify
int msgfile_sign_verify(char *filename, char *githubId, char *passphrase){
	char cmd[BUFF_SIZE];
	char buffer[BUFF_SIZE];
	char cmd_temp[100];
	char cmd_result[30];
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




