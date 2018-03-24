
#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<fcntl.h>
#include 	<signal.h>
#include 	<sys/socket.h>
#include 	<sys/file.h>
#include 	<netinet/in.h>
#include 	<time.h>

/*
   
   1. OnionMessenger 에 오신걸 환영합니다! 유저이름을 입력해 주세요! : eternalklaus
   [isValidGithubID] // 유저이름이 서버에 있는이름인지 검증
	  - 그런 유저이름은 존재하지 않습니다...
	  - 유저이름이 유효합니다
	    [isAuthedUser] // 로그인을 시도합니다.
		- 로그인 성공 -------------------------------2로
		- 로그인 실패 -------------------------------종료
		  
   

   2. 성공적으로 로그인 하였습니다!
   while(1)   
     1. 유저 목록확인
     2.전송 기능
       1. 전송대상 유저 선택
	   2. 전송대상 파일 선택
	   3. 전송하시겠습니까? yes/no
	   4. exit
     3. 엑스트라 기능
       1. 유저 숨기기
	   2. 
     4. 유저 로그아웃 - 유저 삭제, pub파일삭제, .tmp파일들 삭제
 
*/



int isValidGithubID(char *githubID){
	// 성호님 구현
	return 1;
}




int main(int argc, char *argv[]){

	int port;
	srand(time(NULL));
	port = 40000+rand()%10000; // randomly selected port : 40000~50000
	if (argc!=3){
		printf("[USAGE] ./OnionMessenger [IP] [githubID]\n");
		exit(1);
	}
	
	// 서버에다가 { [ip], [port], [github ID] } 를 등록...
	if (!registerUser(argv[1], port, argv[2])){ 
		printf("[ERROR] please confirm your githubID!\n");
		exit(1);
	}
	
	// 서버에서 유저들의 { [ip], [port], [github ID] } 목록을 얻어온다...
	if (!printUserlist()){
		printf("[ERROR] dbserver doesn't works! :( \n");
		exit(1);
	}
	
	// [ip], [port] 서버를 오픈한다
	// 메시지를 받으면 msg_send()으로  
	runServer(port); 
	
	
	printf("test");
}
	