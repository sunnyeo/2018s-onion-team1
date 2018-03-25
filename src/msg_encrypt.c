#include <stdio.h>
#include <stdlib.h>

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>

#include <arpa/inet.h> /* For htonl() */

// 키를 register 한 상태에서, 그 상대방의 키를 이용해서 
// 로컬에 있는 testfile.txt를 암호화하는 함수 
// 로컬에서 testfile.txt를 암호화해서 testfile_encrypt.txt 파일을 만드는 함수


// gpg --import FileName
// 해서 키 임폴트해오고 gpg --recipient bob --encrypt filename 로 할 수 있습니다. 
char *msgfile_encrypt(파일이름, 상대방의 githubID){
// 민철님 구현 부탁드려요~
}

char *msgfile_encrypt_auth(파일이름){
	// A가 E에게 메시지를 보낸다치면
	// 파일을 A자신의 프라이빗키로 암호화해서 보낸다. 
}

char *msgfile_dectypt(char *filename){
	// 상대방이 보낸파일(내 퍼블릭키로 암호화되어 있음)을
	// 나의 private key로 복호화한다.
	// 나는 경유자일수도, 아닐수도 있다. 우선은 복호화한다. 
	
	// 우선 이름은 랜덤스트링으로리턴한다. 나중에 이름 바꾸던가 해야지...
	char cmd[BUFF_SIZE];
	
	snprintf(cmd, BUFF_SIZE, "gpg --passphrase %s --decrypt %s", passphrase, filename);
	system(cmd);
	
	return 0;

}

// 성호님에게 할당~!
char *msgfile_decrypt_auth(파일이름, 보낸이의githubID){
	// 내가 메시지를 까봐서 수신자가 [보낸이의githubID]임을 확인을 했는데
	// 진짜로 [보낸이의githubID]에게 왔는지 확인하려면
	// [파일이름]이 진짜로 복호화되는지 확인해야 함
}






