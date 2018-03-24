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
	
	[메시지 원본]
	line1   : final
	line2   : text/file  // 파일타입 
	line3   : [filename] // 만약 텍스트가 아니라면, 파일의 이름이 들어간다. 
	line3 ~ : 메시지
	*/

int msg_read2shell(char *filename){
	char command[100];
	sprintf(command, "cat %s", filename);
	system(command);
}

// Usage : append_relayinfo("foo.txt", "127.0.0.1", 44444);
int msg_append_relayinfo(char *filename, char *relay_ip, int relay_port){
	char command[100];
	
	sprintf(command, "sed -i '1i%d' %s", relay_port ,filename); // line 2 : 포트정보 추가
	system(command);
	sprintf(command, "sed -i '1i%s' %s", relay_ip ,filename); // line 1 : IP정보 추가
	system(command);
}


// Usage :  msg_extract_relayinfo("foo.txt",&relay_ip,&relay_port) <- char *relay_ip, *relay_port; (Call by reference로 읽어온다)
int msg_extract_relayinfo(char *filename, char **relay_ip, char **relay_port){
    FILE * fp;
    size_t len = 0;
	char command[100];
    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
	getline(relay_ip, &len, fp);   // line1에서 IP읽어서 relay_ip에저장
	getline(relay_port, &len, fp); // line2에서 port읽어서 relay_port에저장

	// IP, Port를 읽어온 후에는 두줄을 삭제한다.
	sprintf(command, "sed -i '1d' %s", filename);
	printf("%s",command);
	system(command);
	system(command);
	
    fclose(fp);
}


// 파일을 받으려는 호스트 : nc -l 1234 > foo.txt
// 파일을 보내려는 호스트 : nc 1.2.3.4 1234 < foo.txt
int msg_send(char *filename, char* relay_ip, int relay_port){ 
	char command[100];
	sprintf(command, "nc %s %d < %s", relay_ip, relay_port, filename); 
	printf("%s\n",command);
	system(command);
	return 1;
}


// 복호화된 파일이 들어간다.
int msg_isthisfinal(char *filename){ 
	char command[100];
	char line1[10];  // final or not
	char linetmp[100];
	FILE * fp;
    size_t len = 0;
    fp = fopen(filename, "r");
    if (fp == NULL){
		printf("[msg_send] 파일 읽기에 실패하였습니다.");
        exit(EXIT_FAILURE);
	}
	fgets(line1, sizeof(line1), fp ); 
	if(!strncmp(line1, "final",5)) {
		printf("[msg_send] final destination arrived!\n"); 
		return 1;
	}
	return 0;
}

// 복호화된 파일이 들어간다. 
int msg_finalProcess(char *filename){
	char command[100];
	char line1[10];  // final or not
	char line2[10];  // 
	char line3[100];
	char linetmp[100];
	FILE * fp;
    size_t len = 0;
	
	fp = fopen(filename, "r");
    if (fp == NULL){
		printf("[msg_send] 파일 읽기에 실패하였습니다.");
        exit(EXIT_FAILURE);
	}
	
	fgets(line1, sizeof(line1), fp );  // PASS(msg_isthisfinal)
	fgets(line2, sizeof(line2), fp );  // 
	if(!strncmp(line1, "final",5)) {
		printf("[msg_send] final destination arrived!\n"); 
		return 1;
	}
	
}
int main(){
	msg_isthisfinal("jiwon.txt");
}


