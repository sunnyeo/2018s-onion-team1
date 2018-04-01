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
	if(!pkey) return 0; // cannot access to keyid
	
	printf("pkey is %s\n", pkey);
	
	// search keyid
	snprintf(cmd, 256, "gpg --verify %s 2> signature.tmp", filename); system(cmd);
	fp = fopen("signature.tmp", "r"); //read write and create
	while(fgets(buf, 256, fp)){
		skey = strstr(buf, "key ID ");
		if(skey) break;
	}
	fclose(fp);
	if(!skey) return 0; // file isn't signed
	
	skey += strlen("key ID ");
	printf("skey is %s\n",skey);
	
	system("sudo rm signature.tmp");
	
	if(!strncmp(pkey,skey,8)){ // sign verified
		snprintf(cmd, 256, "gpg --output %s.tmp --decrypt %s 2>/dev/null", filename, filename); system(cmd);
		snprintf(cmd, 256, "sudo mv %s.tmp %s", filename, filename); system(cmd);
		return 1;
	}
	else{
		return 0;
	}
		
	
	// githubid를 입력하면
	// 613B1F0C 이걸얻어와서
	// gpg --verify onion.sig 이명령어를 때려서 <- user2에서 해도 똑같이 뜸. 
	// gpg: Signature made Sun 01 Apr 2018 04:04:35 AM EDT using RSA key ID 613B1F0C <-- 
	// 이문자열을 얻어오고 비교하기. 
	
	// 원본은 참고로 이렇게 얻을수가 있음
	// gpg --output onion.dec --decrypt onion.sig 
	// 퍼블릭키가 등록이되어있는경우 이걸로 원본을 볼수있지만, 그렇지않은경우에는 디크립트가 안됨.....
	// 걍 디크립트 안되게 해야겠다. 
	
	/*
	if(!to_exist_publickey(githubId))
		exit(202);
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

	// [Hmm...] gpg  --output onion.sigdec --passphrase user1 --decrypt onion.sig
	snprintf(cmd, BUFF_SIZE, "gpg --passphrase %s --decrypt %s > %s.tmp", passphrase, filename, filename);
	system(cmd);
	snprintf(cmd, BUFF_SIZE, "mv %s.tmp %s", filename, filename);
	system(cmd);
	
	if(fsize(filename)==0) return 0;
	else return 1;
	*/
}
int main(){
	if(msgfile_sign_verify("onion.sig", "user1", "??")){
		printf("verified! and made onion.sig undecrypted!!!");
	}
	else
		printf("fail...");
}
