// @userlist
// @adduser "192.168.0.1 2222 eternalklaus"
// @deleteuser eternlaklaus
// 

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

#define _CRT_SECURE_NO_WARNINGS    // strtok 보안 경고로 인한 컴파일 에러 방지



// @usetlist
char *Userlist(){ // 이걸 호출한 유저는 OnionUser.db를 로컬에 다운로드받아햐 함. 
	   int fd;
	   char *buff = (char*)malloc(BUFF_SIZE); // [BUG PATCH]
	   fd=open("OnionUser.db",O_RDONLY);
	   read(fd,buff,BUFF_SIZE-1);
	   close(fd);
	   return buff;
}

// @adduser
int addUser(char *IpPortGithubId) { // char userIp, int userPort, char *githubID
    // OnionUser.db 에 str 한줄 추가
	// 첫줄에 라인 추가 : sed -i '1itask goes here' lll.txt
	char command[100];
	sprintf(command, "sed -i '1i%s ' %s", IpPortGithubId ,"OnionUser.db");  
	printf("addUser command : %s\n", command);
	system(command);
	
	return 1; 
}

// @deleteuser
int deleteUser(char *githubID){
	char command[100];
	
	//sed -i '/eternalklaus/d' aaa.txt 
	sprintf(command, "sed -i '/ %s /d' %s", githubID ,"OnionUser.db"); //추가띄어쓰기
	printf("deleteUser command : %s\n", command);
	system(command);
	
	return 1;
}

// [TODO] : 주기적으로 OnionUser 목록에 있는 IP에다가 쿼리날려서, 유저가 살아있는지 죽어있는지 확인해야 함!
int run_dbserver(int dbserver_port){
   int   server_socket;
   int   client_socket;
   int   client_addr_size;

   struct sockaddr_in   server_addr;
   struct sockaddr_in   client_addr;

   char   buff_rcv[BUFF_SIZE+5];
   char   buff_snd[BUFF_SIZE+5];

   server_socket  = socket( PF_INET, SOCK_STREAM, 0);
   if( -1 == server_socket)
   {
      printf( "server socket 생성 실패\n");
      exit( 1);
   }

   memset( &server_addr, 0, sizeof( server_addr));
   server_addr.sin_family     = AF_INET;
   server_addr.sin_port       = htons(dbserver_port);           
   server_addr.sin_addr.s_addr= htonl(INADDR_ANY);

   if( -1 == bind( server_socket, (struct sockaddr*)&server_addr, sizeof( server_addr) ) )
   {
      printf( "bind() 실행 에러\n");
      exit( 1);
   }

   if( -1 == listen(server_socket, 5))
   {
      printf( "listen() 실행 실패\n");
      exit(1);
   }

   while(1)
   {
      client_addr_size  = sizeof( client_addr);
      client_socket     = accept( server_socket, (struct sockaddr*)&client_addr, &client_addr_size);

      if ( -1 == client_socket)
      {
         printf( "클라이언트 연결 수락 실패\n");
         exit(1);
      }

      read(client_socket, buff_rcv, BUFF_SIZE);
      printf("receive: %s\n", buff_rcv);
      
	  
	  // [TODO] 명령어 전송할 때 ""으로 감싸서 보내기
	  // 서버로의 커멘드 처리 부분
	  if (!strncmp(buff_rcv,"@adduser",strlen("@adduser"))){       // ex) @adduser ip port githubID 
         addUser(buff_rcv+strlen("@adduser")+1);                 
		 printf("[DBSERVER] 환영합니다!\n[DBSERVER] %s\n",buff_rcv+strlen("@adduser")+1); // 서버 프린트
		 sprintf(buff_snd, "[DBSERVER] 환영합니다!\n[DBSERVER] %s\n",buff_rcv+strlen("@adduser")+1); // 유저 프린트
	  }
	  
	  if (!strncmp(buff_rcv,"@deleteuser",strlen("@deleteuser"))){ // ex) @deleteuser githubID 
          deleteUser(buff_rcv+strlen("@deleteuser")+1);
		  printf("[DBSERVER] Username : %s 가 현재 접속리스트에서 제거됩니다. \n",buff_rcv+sizeof("@deleteuser"));  // 서버 프린트  (buff_rcv+sizeof("@deleteuser")+1 하면 왜 짤리지?)
		  sprintf(buff_snd, "[DBSERVER] Username : %s 가 현재 접속리스트에서 제거됩니다. \n", buff_rcv+sizeof("@deleteuser")); // 유저 프린트
		 
	  }
	  
	  if (!strncmp(buff_rcv,"@userlist",strlen("@userlist"))){
         sprintf(buff_snd, "%s", Userlist()); 
		 // 유저측에서는 파일 다운로드 후 저장 - dbserver_client.c 에 구현됨
	  }

	  //클라이언트 소켓에 메시지 전송
	  write(client_socket, buff_snd, strlen(buff_snd)+1);  
      close(client_socket);
	  
	  
   }
}

int main()
{
	run_dbserver(4000); .// 4000번포트사용
}