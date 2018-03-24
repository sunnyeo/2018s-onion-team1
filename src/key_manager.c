#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>

#define  BUFF_SIZE   1024


int download_pubkey(char *githubId){
        char *url = "https://raw.githubusercontent.com/KAIST-IS521/2018-Spring/master/IndividualKeys/";
		char cmd[BUFF_SIZE];
		char githubIdpub[100];
		int ret;
        snprintf(cmd, BUFF_SIZE, "wget %s%s.pub", url, githubId);
        snprintf(githubIdpub, BUFF_SIZE,"%s.pub", githubId);
		ret = access(githubIdpub, F_OK);
		if(ret) // 존재하지 않는다면 다운받는다
		{
			printf("%d",ret);
			system(cmd);
		}
	return 0;	
}

// 로컬에 현재 [githubId.pub]에 있는 상태에서, 그 키를 등록한다. 
// developer : MincheolSon
int register_pubkey(char *githubId){
	
    char cmd[BUFF_SIZE];
    snprintf(cmd, BUFF_SIZE,  "gpg --import %s.pub", githubId);
    system(cmd);
}


// 로컬에 현재 [private key]가 있는 상태에서, 그 키를 등록한다.
// developer : hansh09
int register_private_key(char *privfile){
	char cmd[BUFF_SIZE];

	snprintf(cmd, BUFF_SIZE, "gpg --allow-secret-key-import --import %s", privfile);
	system(cmd);
}


// [TODO]
// ex) 로컬에 hansh17.pub 파일이 있는 상태에서, publickey 문자열(6DBC89AE)을 리턴하는 함수
char get_pubkey(char *githubID){
	/*
    첫번째시도
    Laura$ gpg --import hansh17.pub 
    gpg: key 6DBC89AE: public key "Seongho Han (SH) <hansh09@kaist.ac.kr>" imported
    gpg: Total number processed: 1
    
    두번째시도
    Laura$ gpg --import hansh17.pub 
    gpg: key 6DBC89AE: "Seongho Han (SH) <hansh09@kaist.ac.kr>" not changed
    
    "gpg: key" 문자열 다음으로 파싱해오면될것같은데...
    */
	char *publickey; 
	return publickey;
}


// 그리고 메신저에서 eternalklaus 본인인지 아닌지 확인하는 부분도 넣기
int auth_user(char *githubId){
	// ex) hansh17 을 입력했을 때, 
	//     현재 머신에 hansh17의 private key가 등록된 상태라면? ---> 본인인증 성공(1리턴)
	//     현재 머신에 다른사람의  private key가 등록된 상태라면? ---> 본인인증 실패(0리턴)
}



int main(int argc, char *argv[]){
		
	//addUser("127.0.0.1", 12345, "eternalklaus");
	//addUser("127.0.0.1", 12345, "hansh");
	download_pubkey("eternalklaus");
	
	//dbserver_sendcommand(argv[1]);
}