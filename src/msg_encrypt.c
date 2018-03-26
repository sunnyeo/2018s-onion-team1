#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>

#include <arpa/inet.h> /* For htonl() */


// gpg --import FileName
// 해서 키 임폴트해오고 gpg --recipient bob --encrypt filename 로 할 수 있습니다. 
// 그 상대방의 공개키를 이용해서 로컬에 있는 testfile.txt를 암호화하는 함수 
// developer : MincheolSon
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
    fgets(cmd_result, 30, import);
    temp = strstr(cmd_result, "key");
    key = strstr(temp," ");
    temp = strstr(key, ":");
    *temp = NULL;
    key=strncpy(key_temp, key+1, 15);

    pclose(import);

	// create a random string (need time.h)
	srand(time(NULL));

	char randstr[10];
	for(i=0; i<10;i++)
	{
		snprintf(randstr+i,2,"%c", rand()%26+65);
	}

	// message encryption
    snprintf(cmd, 100, "gpg --armor -o %s.txt --encrypt --recipient %s %s", randstr, key, file_name);
	printf("cmd ::::: %s\n",cmd);
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

// developer : hansh09 [진행중]
int msgfile_dectypt(char *filename){
	// 상대방이 보낸파일(내 퍼블릭키로 암호화되어 있음)을
	// 나의 private key로 복호화한다.
	// 나는 경유자일수도, 아닐수도 있다. 우선은 복호화한다. 
}


int main(){
	msgfile_sign("jiwon.txt");
	//msgfile_sign_verify()
}





