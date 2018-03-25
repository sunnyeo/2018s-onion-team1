#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<fcntl.h>
#include 	<signal.h>
#include 	<sys/socket.h>
#include 	<sys/file.h>
#include 	<netinet/in.h>
#include 	<time.h>

//IP구하기위함
#include<netinet/in.h>
#include<arpa/inet.h>
#include <netdb.h>
#include <sys/utsname.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <string.h>

#define  BUFF_SIZE   1024
/*
   
   1. OnionMessenger 에 오신걸 환영합니다! 유저이름을 입력해 주세요! : eternalklaus
   [isValidGithubID] : 유저이름이 서버에 있는이름인지 검증
	   - NO )  그런 유저이름은 존재하지 않습니다...
	   - YES)  유저이름이 유효합니다
	          [isAuthedUser] : 로그인을 시도합니다.
		      - 로그인 성공 -------------------------------2로
		      - 로그인 실패 -------------------------------종료
		  
   
	
   2. 성공적으로 로그인 하였습니다!
   while(1)   
     1. 유저 목록확인
     2. send 기능
       1. 전송대상 유저 선택
	   2. 전송대상 파일 선택
	   3. 전송하시겠습니까? yes/no
	   4. exit
	 3. recv 기능
	   case1 : 전송받은 파일이 final이 아닐때 : 다음사용자에게 relay
	   case2 : 전송받은 파일이 final일때 :
	           1. text일때 : 읽어서 화면에 뿌려주고 파일은 삭제
			   2. file일때 : 읽어서 화면에 "[파일이름]이 도착하였습니다!" 를 뿌려줌.
     3. 엑스트라 기능
       1. 유저 숨기기
	   2. 
     4. 유저 로그아웃 - 유저 삭제, pub파일삭제, .tmp파일들 삭제
 
*/
// 스레드 2개 !! - 받는얘가 백그라운드 스레드!



// 만약 깃허브의 IndividualKey 폴더에 해당 유저가 있다면 1 리턴, 없다면 0을 리턴합니다. 
// developer : hansh09
int isValidGithubID(char *githubId){
	char *url = "https://github.com/KAIST-IS521/2018-Spring/blob/master/IndividualKeys/";
	char cmd[BUFF_SIZE];
	snprintf(cmd, BUFF_SIZE, "wget --spider --timeout=1 %s%s.pub", url, githubId);
	if(!system(cmd))
	{
		return 1;
	}
	else
		return 0;
}


// 로그인시도하는 호스트의 IP를 리턴한다 
// Usage : ipaddr = getHostIP("eth0");
char *getHostIP(char *interface){
	struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];
	
	// 리턴하는 IP주소
	char *ret = (char*)malloc(NI_MAXHOST);
	
    if (getifaddrs(&ifaddr) == -1) 
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) 
    {
        if (ifa->ifa_addr == NULL)
            continue;  

        s=getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
        if((strcmp(ifa->ifa_name,interface)==0)&&(ifa->ifa_addr->sa_family==AF_INET))
        {
            if (s != 0)
            {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }
            // printf("\tInterface : <%s>\n",ifa->ifa_name );
            // printf("\t  Address : <%s>\n", host); 
			strncpy(ret, host, sizeof(host));
        }
    }
    freeifaddrs(ifaddr);
	return ret;
}

int trylogin(char *githubID){
	
	
}
int main(int argc, char *argv[])
{
	char *ipaddr;
	ipaddr = getHostIP("eth0");
	printf("%s",ipaddr);
	
	
}

/*
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
*/
	