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
#include <time.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <pthread.h>
#define  BUFF_SIZE   1024

#define _CRT_SECURE_NO_WARNINGS    // strtok 보안 경고로 인한 컴파일 에러 방지
sem_t semaphore;

int deleteUser(char *githubID);

// developer : eternalklaus
// contributer : MincheolSon, Dauren
int isClientAlive(char *ClientIP, int ClientPort){
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
	char   buff_snd[BUFF_SIZE+5];
    char   buff_rcv[BUFF_SIZE+5];
    int rnum[3] = {0,};
    int i;
    char buf_1[256];
    char buf_2[256];
    FILE *snd;
    FILE *rcv;
    char portnum[16];
 /*
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("ERROR opening socket");
        exit(1);
    }
	memset( &serv_addr, 0, sizeof( serv_addr));
	serv_addr.sin_family      = AF_INET;
    serv_addr.sin_port        = htons(ClientPort);
    serv_addr.sin_addr.s_addr = inet_addr(ClientIP);
	
    srand(time(NULL));
	if(-1==connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)))
        return 0;
*/
    srand(time(NULL));
    for(i=0; i<3; i++) rnum[i] = rand();
    snd = fopen("snd_file","w");
    snprintf(buff_snd, BUFF_SIZE, "alive%d%d%d", rnum[0], rnum[1], rnum[2]);
    fputs(buff_snd, snd);
    fclose(snd);
  //  printf("alive???\n");
    snprintf(buf_1, 256, "timeout 1 nc -l -p %d > %s 2>/dev/null", 4001, "rcv_file");
   // system(buf);
    snprintf(portnum, 16, "%d", ClientPort);
    snprintf(buf_2, 256, "cat snd_file | nc %s %s", ClientIP, portnum);
    system(buf_2);
    system(buf_1);
    rcv = fopen("rcv_file","r");
    fgets(buff_rcv, BUFF_SIZE, rcv);
//	write(sockfd, buff_snd, strlen(buff_snd)+1);
  //  printf("%s\n",buff_snd);
  //  read (sockfd, buff_rcv, BUFF_SIZE);
  //  printf("%s\n", buff_rcv);
   // close(sockfd);
    if (strncmp(buff_snd, buff_rcv, strlen(buff_snd)) == 0){
    //	printf("%s:%d is still alive...\n", ClientIP,ClientPort);
	    return 1;
    }
    else return 0;
}

void alive_check(){
    FILE *fp;
    FILE *delete_list;
    char *ClientIP;
    int ClientPort;
    char *Port_ch;
    char *UserId;
    int ret;
    char fline[100];

    /*
    while(1){
        sleep(1);
        printf("11\n");
       // sem_wait(&semaphore);
        printf("22\n");
        system("cp OnionUser.db OnionUser.db.list");
        fp = fopen("OnionUser.db.list","r");    
        delete_list = fopen("DeleteUser","w");
        fgets(fline, 100, fp);
        while(fline[0]!='-'){
            ClientIP = strtok(fline, " ");
            Port_ch = strtok(NULL, " ");
            UserId = strtok(NULL, " ");
            ClientPort = atoi(Port_ch);
            ret = isClientAlive(ClientIP, ClientPort);
            if(!ret) fprintf(delete_list, "%s\n", UserId);
            fgets(fline, 100, fp);
            printf("1313\n");
        }
        fclose(delete_list);
        fclose(fp);
        delete_list = fopen("DeleteUser","r");
        while(!feof(delete_list)){
            fgets(fline, 100, delete_list);
            if(fline[0]=='\n') break;
            printf("565\n");
            deleteUser(fline);
        }
        fclose(delete_list);
        system("rm DeleteUser");
        printf("123\n");
       // sem_post(&semaphore);
    }
    */
    while(1){
        sleep(1);
        system("cp OnionUser.db OnionUser.db.list");
        fp = fopen("OnionUser.db.list","r");
        delete_list = fopen("DeleteUser","w");
        fgets(fline, 100, fp);
        while(fline[0]!='-'){
            ClientIP = strtok(fline, " ");
        //    printf("ip : %s\n", ClientIP);
        //    Port_ch = strstr(fline, "\0");
        //    printf("tmpe : %s\n", Port_ch);
            Port_ch = strtok(NULL, " ");
         //   printf("port : %s\n", Port_ch);
         //   UserId = strstr(Port_ch, "\0");
            UserId = strtok(NULL, " ");
            ClientPort = atoi(Port_ch);
            ret = isClientAlive(ClientIP, ClientPort);
            if(!ret) fprintf(delete_list, "%s\n", UserId);
            else printf("%s is still alive\n", UserId);
            fgets(fline, 100, fp);
        }
        fclose(delete_list);
        fclose(fp);
        delete_list = fopen("DeleteUser","r");
    //    printf("22\n");
        while(1){
            fgets(fline, 100, delete_list);
            if(feof(delete_list)!=0) break;
            if(fline[0]=='\n') break;
        //    printf("565\n");
            deleteUser(fline);
        }
    //    printf("33\n");
        fclose(delete_list);
        system("rm DeleteUser");
        
    }
}

// @userlist
char *Userlist(){ // User should download result as OnionUser.db.tmp
	   int fd;
	   char *buff = (char*)malloc(BUFF_SIZE); 
	   fd=open("OnionUser.db",O_RDONLY);
	   read(fd,buff,BUFF_SIZE-1);
	   close(fd);
	   return buff;
}

// @adduser
int addUser(char *IpPortGithubId) { // char userIp, int userPort, char *githubID
	char command[100];
	sprintf(command, "sed -i '1i%s ' %s", IpPortGithubId ,"OnionUser.db");  
	system(command);
	return 1; 
}

// @deleteuser
int deleteUser(char *githubID){
	char command[100];
	sprintf(command, "sed -i '/ %s/d' %s", githubID ,"OnionUser.db");
	system(command);
	
	return 1;
}

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
      printf( "[DBSERVER] server socket error\n");
      exit( 1);
   }

   memset( &server_addr, 0, sizeof( server_addr));
   server_addr.sin_family     = AF_INET;
   server_addr.sin_port       = htons(dbserver_port);           
   server_addr.sin_addr.s_addr= htonl(INADDR_ANY);

   if( -1 == bind( server_socket, (struct sockaddr*)&server_addr, sizeof( server_addr) ) )
   {
      printf( "[DBSERVER] bind() error\n");
      exit( 1);
   }

   if( -1 == listen(server_socket, 5))
   {
      printf( "[DBSERVER] listen() error\n");
      exit(1);
   }

   while(1)
   {
      // sem_wait(&semaphore);
      client_addr_size  = sizeof( client_addr);
      client_socket     = accept( server_socket, (struct sockaddr*)&client_addr, &client_addr_size);

      if ( -1 == client_socket)
      {
         printf( "[DBSERVER] Client connection failed\n");
         exit(1);
      }

      read(client_socket, buff_rcv, BUFF_SIZE);
      
	  // server command : @adduser, @deleteuser, @userlist
	  if (!strncmp(buff_rcv,"@adduser",strlen("@adduser"))){ 
         addUser(buff_rcv+strlen("@adduser")+1);                 
		 printf("[DBSERVER] User login : %s\n",buff_rcv+strlen("@adduser")+1); 
	  }
	  
	  if (!strncmp(buff_rcv,"@deleteuser",strlen("@deleteuser"))){ 
          deleteUser(buff_rcv+strlen("@deleteuser")+1);
		  printf("[DBSERVER] User logout : %s\n",buff_rcv+sizeof("@deleteuser"));  // 서버 프린트  (buff_rcv+sizeof("@deleteuser")+1 하면 왜 짤리지?)
          
	  }
	  
	  if (!strncmp(buff_rcv,"@userlist",strlen("@userlist"))){
        // sem_wait(&semaphore);
         sprintf(buff_snd, "%s", Userlist());  // user can download [buff_snd] buffer as a file. 
        // sem_post(&semaphore);
	  }
	  write(client_socket, buff_snd, strlen(buff_snd)+1);  
      close(client_socket);
   }
   // [TODO] 다른 프로세스에서는 OnionUser.db 한줄씩 돌면서
   // isClientAlive 실행하기. 
}

int main()
{
   //sem_init(&semaphore, 1, 1);

   // pthread_t th1=0;
    
   // pthread_create( &th1, NULL, alive_thread, NULL);
   // pthread_detach( th1 );
    pid_t pid;

    pid = fork();
    switch(pid)
    {
        case -1 :
            {
                printf("fork fail\n");
                return -1;
            }
        case 0 : alive_check();
        default : run_dbserver(4000); // 4000번 포트사용
    }
   // run_dbserver(4000);
}

