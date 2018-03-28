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

#define _CRT_SECURE_NO_WARNINGS  

	/* 
	참고...
	
	[전송하는 파일 형식]
	line1   : relay IP
	line2   : relay Port
	line3 ~ : 암호화된 message 내용
	*/

	/*
	A->E에서 간다칠때 A는 다음과같은 메시지블록을 준비해야 한다. 
	
	[태초의 메시지 블록]
	line1   : final
	line2   : text/file  // 파일타입 
	line3   : 보낸이의 githubID 
	line4   : [filename] // 텍스트라면 이 라인은 비워둔다. 
	line5 ~ : 보낸이의 githubID의 private key로 암호화된 파일
	*/


// Usage : append_relayinfo("foo.txt", "127.0.0.1", 44444);
int msg_append_relayinfo(char *filename, char *relay_ip, int relay_port){
	char command[100];
	
	sprintf(command, "sed -i '1i%d' %s", relay_port ,filename); // line 2 : 포트정보 추가
	system(command);
	sprintf(command, "sed -i '1i%s' %s", relay_ip ,filename); // line 1 : IP정보 추가
	system(command);
}


// [태초의 메시지 블록] 을 만든다
int msg_append_msginfo(char *filename, char *textORfile,  char *mygithubID){ // 보낸사람은 검증없이 mygithubID를 넣어서 만들고, "받는사람" 측에서 이를 검증한다.  

	char command[100];
	sprintf(command, "sed -i '1i%s' %s", filename ,filename); // line 4 : 파일이름 추가
	system(command);
	sprintf(command, "sed -i '1i%s' %s", mygithubID ,filename); // line 3 : 보낸이의 githubID  추가 
	system(command);
	sprintf(command, "sed -i '1i%s' %s", textORfile ,filename); // line 2 : 파일타입 (text/file) 추가 
	system(command);
	sprintf(command, "sed -i '1ifinal' %s",filename); // line 1 : "final" 추가 
	system(command);
	
}


// relay information 을 추출한 후 2라인을 없앤다. 
// Usage :  msg_extract_relayinfo("foo.txt",&relay_ip,&relay_port) <- char *relay_ip, *relay_port; (Call by reference로 읽어온다)
int msg_extract_relayinfo(char *filename, char **relay_ip, char **relay_port){
    FILE * fp;
    size_t len = 0;
	char command[100];
	char *tmp;
    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
	
	tmp = (char*)malloc(100);
	getline(&tmp, &len, fp);   // line1에서 IP읽어서 relay_ip에저장
	*relay_ip = tmp;

	tmp = (char*)malloc(100);
	getline(&tmp, &len, fp);   // line2에서 port읽어서 relay_port에저장
	*relay_port = tmp;

	sprintf(command, "sed -i '1d' %s", filename);
	printf("%s",command);
	system(command); // 1line 삭제
	system(command); // 2line 삭제
	
    fclose(fp);
}


// 복호화된 파일이 들어간다. 파일자체를 수정하지는 않음. 
int msg_isthisfinal(char *filename){  
	char command[100];
	char line1[10];  // final or not
	char linetmp[100];
	FILE * fp;
    size_t len = 0;
    fp = fopen(filename, "r");
    if (fp == NULL){
		// printf("[msg_send] 파일 읽기에 실패하였습니다.");
        exit(EXIT_FAILURE);
	}
	fgets(line1, sizeof(line1), fp ); 
	if(!strncmp(line1, "final",5)) {
		printf("[msg_send] final destination arrived!\n"); 
		return 1;
	}
	return 0;
}


// final information을 추출한 후  4라인을 없앤다.
// usage : msg_extract_finalinfo("encrypt.enc", &textORfile, &sender_githubID, &original_filename); char *original_filename;
int msg_extract_finalinfo(char *filename, char **textORfile, char **sender_githubID, char **original_filename){
	FILE * fp;
	size_t len = 0;
	char command[100];
	char *tmp;
	
	fp = fopen(filename, "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);
	
	tmp = (char*)malloc(100);
	getline(&tmp, &len, fp);   // final
	
		
	tmp = (char*)malloc(100);
	getline(&tmp, &len, fp);   // text
	*textORfile = tmp;
	
	tmp = (char*)malloc(100);
	getline(&tmp, &len, fp);   // githubID
	*sender_githubID = tmp;

	tmp = (char*)malloc(100);
	getline(&tmp, &len, fp);   // filename
	*original_filename = tmp;

	
	sprintf(command, "sed -i '1d' %s", filename);
	printf("%s\n",command);
	
	system(command); // 1line 삭제
	system(command); // 2line 삭제
	system(command); // 3line 삭제
	system(command); // 4line 삭제
	
	// 파일이름을 original_filename 으로 바꾼다. (그런데.. 아직 암호화된 상태이므로 복호화처리가 필요함.) 
	sprintf(command, "mv %s %s", filename, *original_filename);
	system(command);
	
	fclose(fp);
}


int main(){
	char *sender_githubID;
	char *original_filename;
	char *textORfile;
	
	
	msg_extract_finalinfo("encrypt.enc", &textORfile, &sender_githubID, &original_filename);
	
	printf("textORfile : %s\n",textORfile);
	printf("sender_githubID : %s\n",sender_githubID);
	printf("original_filename : %s\n",original_filename);
	
}