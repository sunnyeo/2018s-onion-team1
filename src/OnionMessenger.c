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


// 유저중 랜덤으로 생성된 Circuit을 리턴한다. (sendernum, receivernum는 제외한다)
int *Onion_selectCircuit(int totalUserNum, int circuit_len, int sendernum, int receivernum)
{
    // 삽입 방식
    int flag[totalUserNum+1];
	int i, j, number, count;
	int *circuit;

	memset( flag, 0, (totalUserNum+1)*sizeof(int) );
	circuit = (int*)malloc(sizeof(int)*(circuit_len-2));
    srand((unsigned int)time(NULL));
	
	// 센더와리시버는제외
	flag[sendernum]=2;
	flag[receivernum]=2;
	
    count = 0;
    do
    {
        number = (rand() % totalUserNum) + 1;
        if( flag[number] == 0 )
        {
            flag[number] = 1;
            count++;
			//printf("Pick! : %d, Count : %d\n",number,count);
        }
    }
    while( count < circuit_len-2 ); //circuit_len 개 만큼 뽑아온다. 

	printf("hahahahahhahaha\n");
	
	j=0;
	for(i=1; i < totalUserNum+1; i++ ){
        if(flag[i]==1){
			printf( "%d ", i );
			circuit[j++]=i;
		}
	}
    printf("\n");

	return circuit;
}




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
int onion_getTotalUsernum(){
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


/*
// len개의 circuit을 구성한다
char** onion_makeCircuit(int circuit_len, char *sender_githubID, char *receiver_githubID){ // 192.168.1.124 4444 eternalklaus 의 문장으로구성된 문자열리스트를 리턴한다. 
	FILE *fp;
	char **lines; // 이걸 동적으로 할당하고 싶음......
	char *line;
	int i=0;
	int linenum,usernum,sendernum,receivernum;

	
	usernum = onion_getTotalUsernum();
	fp = fopen("OnionUser.db.tmp", "r");
	lines = (char**)malloc(sizeof(line) * circuit_len); // 3개짜리 스트링 리스트
	
	
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
				lines[circuit_len-1]=line;
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
*/

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
	
	//void InsertNumber(int totalUserNum, int circuit_len, int sendernum, int receivernum)
	int *rett;
	int i;
	rett = Onion_selectCircuit(10, 5, 3, 4);
	for(i=0;i<5-2;i++){
		printf("%d-->",rett[i]);
	}
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
	