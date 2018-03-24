
#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<fcntl.h>
#include 	<signal.h>
#include 	<sys/socket.h>
#include 	<sys/file.h>
#include 	<netinet/in.h>
#include 	<time.h>

/*
   getchar으로 유저인풋을 받아서
   1: 유저 로그인시도
      Github ID 을 입력해 주세요 : eternalklaus
	  - 그런 유저이름은 존재하지 않습니다...
	  - 로그인 성공
	  
   2: 유저 목록확인
   3. 전송 기능
      1. 전송대상 유저 선택
	  2. 전송대상 파일 선택
	  3. 전송하시겠습니까? yes/no
	  4. exit
   4. 엑스트라 기능
      1. 유저 숨기기
	  2. 
   5. 유저 로그아웃 - 유저 삭제, pub파일삭제, .tmp파일들 삭제
 
*/


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
	