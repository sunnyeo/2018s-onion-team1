#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>

#include <arpa/inet.h> /* For htonl() */
#define BUFF_SIZE 1024

// gpg --import FileName
// 해서 키 임폴트해오고 gpg --recipient bob --encrypt filename 로 할 수 있습니다. 
// 그 상대방의 공개키를 이용해서 로컬에 있는 testfile.txt를 암호화하는 함수 
// developer : MincheolSon
// [TODO] 사용자 인터렉션 no/yes 선택하는거 없애기 : --trust-model always 옵션을 추가할 것!
int msgfile_encrypt(char *file_name, char *githubId){
    
    char cmd[100];
    char cmd_result[30];
    char key_temp[30];
    char *temp;
    char *key;
    FILE *import;
 
    snprintf(cmd, 100, "gpg --import %s.pub 2>&1", githubId);
    import = popen(cmd, "r");
    if(import == NULL)
    {
        printf("There is no %s.pub file\n", githubId);
        return -1;
    }
	
    if(!fgets(cmd_result, 29, import)) { // [취약점 패치] [@eternalklaus] cmd_result가 없을시 segmentation fault가 발생하는 문제 해결
        printf("There is no %s.pub file\n", githubId);
        return -1;
	}
	printf("haha\n");
    temp = strstr(cmd_result, "key");
    key = strstr(temp," ");
    temp = strstr(key, ":");
    *temp = NULL;
    key=strncpy(key_temp, key+1, 15);

    pclose(import);

    snprintf(cmd, 100, "gpg --trust-model always --armor --encrypt --recipient %s %s", key, file_name); 
    system(cmd);

    return 0;
}


// developer : hansh09
int  msgfile_sign(char *filename, char *passphrase){
// 메세지 파일을 서명한다. 
	char cmd[BUFF_SIZE];
	snprintf(cmd, BUFF_SIZE, "gpg --passphrase %s --sign %s", passphrase, filename);
	system(cmd);
// encryption과 함께 사용하고 싶으면 gpg --armor -o [output.txt] --passphrase [passphrase] --sign --encrypt -r [recipient's ID] [input.txt] 를 사용한다. 
} 


// developer : hansh09
// [TODO] [성호] [오전 3:51] 넵 verify할 대상이 sign & encryption 같이 되었다고 생각하고 구현해서요 
//        [성호] [오전 3:52] 염두해둬야될거같습니다
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
	temp = strstr(key, ":");
	*temp = NULL;
	key = strncpy(key_temp, key+1, 15);
	pclose(import);

	snprintf(cmd, BUFF_SIZE, "gpg --passphrase %s --decrypt %s > info.txt 2>&1", passphrase, filename);
	system(cmd);
	FILE *fp = fopen("info.txt", "r");


	while(fgets(buffer, sizeof(buffer), fp) != NULL){
		if(strstr(buffer, key)!=NULL){
			return 1;
		}
	}
	
	if(fp){
		fclose(fp);
	}

	return 0;
}


// developer : hansh09
int msgfile_decrypt(char *filename, char *passphrase){
	// 상대방이 보낸파일(내 퍼블릭키로 암호화되어 있음)을
	// 나의 private key로 복호화한다.
	// 나는 경유자일수도, 아닐수도 있다. 우선은 복호화한다. 
	
	char cmd[BUFF_SIZE];
	
	snprintf(cmd, BUFF_SIZE, "gpg --passphrase %s --decrypt %s", passphrase, filename);
	system(cmd);
	
	return 0;

}

int main(){
	// 다른사람의 아이디를 가져와서 
	download_pubkey("AhnMo");
	msgfile_encrypt("jiwon.txt","AhnMo");
}





