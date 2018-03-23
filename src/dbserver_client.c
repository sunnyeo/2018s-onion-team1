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


/*
[TODO] githubId를 입력받아서, 서버에서 pub파일을 다운받는 함수

int download_pubkey(char *githubId){
	// 깃허브 서버에서 githubId에 해당하는 키들을 로컬로 다운로드한다. (wget 같은걸 이용하면 될것같아요)
	// 키 경로 : https://github.com/KAIST-IS521/2018-Spring/blob/master/IndividualKeys/[githubId.pub]
}


[TODO] githubId.pub이 있는 상태에서, 그 키를 import하는 함수

int register_key(char *githubId){
	// 로컬에 현재 [githubId.pub]에 있는 상태에서, 그 키를 등록한다. 
	
	// 키를 등록하는 커멘드 명령어는 아래를 참고
	Laura$ gpg --import hansh17.pub 

	}
*/




int dbserver_sendcommand(char *command){
   int   client_socket;
   struct sockaddr_in   server_addr;
   char   buff[BUFF_SIZE+5];
   int    fd;



   client_socket  = socket( PF_INET, SOCK_STREAM, 0);
   if( -1 == client_socket)
   {
      printf( "[DBSERVER] socket 생성 실패\n");
      exit( 1);
   }

   memset( &server_addr, 0, sizeof( server_addr));
   server_addr.sin_family     = AF_INET;
   server_addr.sin_port       = htons( 4000);
   server_addr.sin_addr.s_addr= inet_addr( "127.0.0.1");

   if( -1 == connect( client_socket, (struct sockaddr*)&server_addr, sizeof( server_addr) ) )
   {
      printf( "[DBSERVER] 접속 실패\n");
      exit( 1);
   }
   
   write( client_socket, command, strlen(command)+1);      // +1: NULL까지 포함해서 전송
   read ( client_socket, buff, BUFF_SIZE);
   
   if(!strncmp(command,"@userlist",strlen("@userlist"))){
	   fd=open("OnionUser.db.tmp", O_RDWR | O_TRUNC | O_CREAT, 0777);// 읽어온 buff를 파일에 저장
	   write(fd, buff, strlen(buff));
	   close(fd);
   }

   printf( "%s\n", buff);
   close( client_socket);
   
   return 0;
}

int main(int argc, char *argv[]){
	
	dbserver_sendcommand(argv[1]);
	//addUser("127.0.0.1", 12345, "eternalklaus");
	//addUser("127.0.0.1", 12345, "hansh");
    
}


	