
#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<fcntl.h>
#include 	<stdlib.h>
#include 	<signal.h>
#include 	<sys/socket.h>
#include 	<sys/file.h>
#include 	<netinet/in.h>
#include    <time.h>

#include    "str_def.h"



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
		
	
	
	printf("test");
}
	