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


//OnionUser.db.tmp 에 있는 유저의 public key들을 모두 등록한다
int onion_registerAllPubKey(){
	FILE *fp;
	char line[100];
	char *githubID;
	fp = fopen("OnionUser.db.tmp", "r");  // 파일 열기
	int i=0;
	
	if (fp == NULL){
	   printf("유저목록이 없어서 못함. 하지만 유저목록이 없을리가 없지\n");
	}
	else{
		while(!feof(fp)){ // 파일의 끝이 아니라면
			fgets(line, 100, fp);
			if(line[0]=='-')
				break;    // end of OnionUser.db file	
			printf("%s",line);
			
			githubID = strchr(line, ' ')+1;
			githubID = strchr(githubID, ' ')+1;
			
			// 엔터 제거
			githubID[strlen(githubID)-1] = '\0';
			printf("%s\n",githubID);
			
			//key_manager.c 에서 가져옴
			download_pubkey(githubID);
			register_pubkey(githubID);
		}
		printf("사용자 키 등록을 완료하였습니다\n");
	}
	return 0;
}

int onion_showUserlist(){
	// OnionUser.db.tmp 읽어서 화면에 뿌려줌... 우선은 printf 로만 구현해보자
}

// 현재 로그인한사람들 Num을 리턴하는 함수
int onion_getUsernum(){
	FILE *fp;
	fp = fopen("OnionUser.db.tmp","r");
	int i=0;
	char line[100];
	if (fp == NULL){
	   printf("유저목록이 없어서 못함. 하지만 유저목록이 없을리가 없지\n");
	}
	else{
		while(!feof(fp)){ // 파일의 끝이 아니라면
			fgets(line, 100, fp);
			if(line[0]=='-')
				break;    // end of OnionUser.db file
			i++;
		}
	}
	return i;
}



// len개의 circuit을 구성한다
char** onion_makeCircuit(int len, char *sender_githubID, char *receiver_githubID){ // 192.168.1.124 4444 eternalklaus 의 문장으로구성된 문자열리스트를 리턴한다. 
	FILE *fp;
	char **lines; // 이걸 동적으로 할당하고 싶음......
	char *line;
	int i=0;
	int linenum,usernum,sendernum,receivernum;
	
	
	
	usernum = onion_getUsernum();
	fp = fopen("OnionUser.db.tmp", "r");
	lines = (char**)malloc(sizeof(line) * len); // 3개짜리 스트링 리스트
	
	
	if (fp == NULL){
	   printf("유저목록이 없어서 못함. 하지만 유저목록이 없을리가 없지\n");
	}
	else{
		linenum=1; //라인은 1에서 시작
		while(!feof(fp)){
			line = (char*)malloc(100); //동적으로 할당
			fgets(line, 100, fp);
			line[strlen(line)-1] = '\0';
			if(strstr(line,sender_githubID)){
				lines[0]=line;
				sendernum=linenum;
			}
			else if(strstr(line,receiver_githubID)){
				lines[len-1]=line;
				receivernum=linenum;
			}
			else{
				free(line); //읽어온 라인 쓸모없음. 
			}
			if(line[0]=='-')
				break;
			linenum++;
		}
		printf("sendernum : %d, receivernum : %d\n",sendernum, receivernum);
		
		fseek(fp,0,SEEK_SET); //파일의 시작으로 되돌아온다.

		
		// 1~num 중에 sendernum, receivernum 을 제외하고
		// [num-2] 개의 랜덤넘버를 뽑는다. 
		
		randnum[num-2];
		
		sort(randnum);
		
		i=1;
		linenum=1;
		while(!feof(fp)){
			line = (char*)malloc(100);
			fgets(line, 100, fp);
			line[strlen(line)-1] = '\0';
			
			if(linenum == randnum[i]){
				
			}
			else{
				
			}
			linenum++;
		}
		
	}
	return lines;
	
}


/* 프로토타입...
   1. OnionMessenger 에 오신걸 환영합니다! 유저이름과 passphrase을 입력해 주세요! : eternalklaus, [TODO] passphrase
   [isValidGithubID] : 유저이름이 서버에 있는이름인지 검증
	   - NO )  그런 유저이름은 존재하지 않습니다...--------------종료
	   - YES)  유저이름이 유효합니다
	          [isAuthedUser] : 로그인을 시도합니다. + [TODO] passphrase 가 eternalklaus의 것인지 아닌지 검증
		      - 로그인 성공 -------------------------------2로
		      - 로그인 실패 -------------------------------종료
		  
   
	
   2. 성공적으로 로그인 하였습니다!
      2.1 자동으로 서버에 githubID, IP, port 등록하기  : dbserver_sendcommand("@adduser IP Port GithubID")
      2.2 자동으로 서버에서 유저목록 가져와서 저장하기      : dbserver_sendcommand("@userlist")  (참고. OnionUser.db.tmp 가 로컬에 저장됨)
	  2.3 자동으로 유저목록들 키를 모두 등록하기          : onion_registerAllPubKey()
   
	
   while(1)   
	 3. 대기중..............대기할때 주기적으로 유저목록 갱신
        nc로 리쓰닝중........쓰레드1는 백그라운드리쓰닝 / 쓰레드2는 포그라운드처리
     
	 4. 유저 목록 보여주는 기능 (이거호출할때 목록 자동 업데이트함. 그래서 사람이 직접 업데이트할필요 없음)
        2.2 ㅇㅇ
		2.3 ㅇㅇ
	    4.1 [TODO] 그냥 단순히 로컬에있는 OnionUser.db.tmp 읽어서 화면에 뿌려줌
	 5. send 기능
	   5.0 유저 목록 갱신하고 목록 보여줌
	       2.2 ㅇㅇ
		   2.3 ㅇㅇ
       5.1 전송대상 유저의 GithubID를 입력해 주세요 : eternalklaus
	       5.1.1 유저가 있는 경우 : 유저선택 완료.
		   5.1.2 유저가 없는 경우 : 다시 입력해 주세요!
		         GOTO 5.1
	   5.2 전송대상 파일의 이름을 입력해 주세요.
	       5.2.1 파일이 있는 경우 : 파일선택 완료.
		   5.2.2 파일이 없는 경우 : 다시 입력해 주세요!
		         GOTO 5.2
	   5.3 전송하시겠습니까? yes/no
	       5.3.1 yes
		         [TODO] OnionUser.db.tmp 에서 Circuit 구성 (ex. A->C->B)
				        메시지 포장 포장 포장... [TODO] 메시지 암호화할때 현재 .enc, .asc 이런식으로 확장자가 붙는파일을 생성한다. [특정이름]을 정해두고, 암호화/복호화함수에서 그 [특정이름]을 리턴하도록 하면?
				 메시지 전송
				 GOTO 3 ([TODO] 메시지가 잘 도착했는지 아닌지 receiver가 응답줘야함... 헤더가 success인 파일을 답변으로 보낼까?)
		   5.3.2 no
		         GOTO 3
	 6. recv 기능
	    6.1 nc로 데이터가 하나 배달된 경우
		6.2 final 이 아닌경우 - 다음사용자에게 relay 
		6.3 final 인    경우 - 
	        6.3.1. text일때 : 읽어서 화면에 뿌려주고 파일은 삭제 [TODO]
			6.3.2. file일때 : 읽어서 화면에 "[파일이름]이 도착하였습니다!" 를 뿌려줌. [TODO]
     7. 엑스트라 기능
       1. 네트워크에서 유저 숨기기.. (유저가 네트워크 대역폭 짠돌이일경우?)
	   2. 
     8. [TODO]유저 로그아웃 - 유저 삭제, pub파일삭제, .tmp파일들 삭제
 
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
// Usage : char *ipaddr = getHostIP("eth0");
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



int main(int argc, char *argv[])
{
	char **lines;
	int i;

	lines = onion_makeCircuit(3, "eternalklaus", "hansh17");
	for(i=0;i<3;i++)
		printf("%s-->",lines[i]);

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
	