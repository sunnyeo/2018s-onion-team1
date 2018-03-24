#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define  BUFF_SIZE   1024



int download_pubkey(char *githubId){
		CURL *curl;
        FILE *fp;
        CURLcode res;
        char url[1000] = "https://raw.githubusercontent.com/KAIST-IS521/2018-Spring/master/IndividualKeys/";
		strcat(url, githubId);
		strcat(url, ".pub");
	    char outfilename[FILENAME_MAX]="";
		strcat(outfilename,githubId);
		strcat(outfilename,".pub");
        curl = curl_easy_init();
        if (curl)
        {
	   		fp = fopen(outfilename,"wb");
			curl_easy_setopt(curl, CURLOPT_URL, url);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
			fclose(fp);
        }

	return 0;	
}


	// 깃허브 서버에서 githubId에 해당하는 키들을 로컬로 다운로드한다. (wget 같은걸 이용하면 될것같아요)
	// 키 경로 : https://github.com/KAIST-IS521/2018-Spring/blob/master/IndividualKeys/[githubId.pub]
}

/*

[TODO] githubId.pub이 있는 상태에서, 그 키를 import하는 함수

int register_key(char *githubId){
	// 로컬에 현재 [githubId.pub]에 있는 상태에서, 그 키를 등록한다. 
	
	// 키를 등록하는 커멘드 명령어는 아래를 참고
	Laura$ gpg --import hansh17.pub 

	}
	*/

int main(int argc, char *argv[]){
		
	//addUser("127.0.0.1", 12345, "eternalklaus");
	//addUser("127.0.0.1", 12345, "hansh");

   int   client_socket;

   struct sockaddr_in   server_addr;

   char   buff[BUFF_SIZE+5];

   client_socket  = socket( PF_INET, SOCK_STREAM, 0);
   if( -1 == client_socket)
   {
      printf( "socket 생성 실패\n");
      exit( 1);
   }

   memset( &server_addr, 0, sizeof( server_addr));
   server_addr.sin_family     = AF_INET;
   server_addr.sin_port       = htons( 4000);
   server_addr.sin_addr.s_addr= inet_addr( "127.0.0.1");

   if( -1 == connect( client_socket, (struct sockaddr*)&server_addr, sizeof( server_addr) ) )
   {
      printf( "접속 실패\n");
      exit( 1);
   }
   
   // 처음 argv[1]에다가 메시지 전송해줘야 세그폴안뜸~!
   write( client_socket, argv[1], strlen( argv[1])+1);      // +1: NULL까지 포함해서 전송
   read ( client_socket, buff, BUFF_SIZE);
   printf( "%s\n", buff);
   close( client_socket);
   
   return 0;
}


	
