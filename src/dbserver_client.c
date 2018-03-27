#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/stat.h>

#define  BUFF_SIZE   1024




int dbserver_sendcommand(char *command){
   int   client_socket;
   struct sockaddr_in   server_addr;
   char   buff[BUFF_SIZE+5];
   int    fd;

   client_socket  = socket( PF_INET, SOCK_STREAM, 0);
   if( -1 == client_socket)   {
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
}


	
