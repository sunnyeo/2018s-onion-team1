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
	line3   : 송신자
	line4   : [filename] // 만약 텍스트가 아니라면, 파일의 이름이 들어간다. 
	line5 ~ : 메시지
	*/

int msg_read2shell(char *filename){
	char command[100];
	sprintf(command, "cat %s", filename);
	system(command);
}


// 파일을 받으려는 호스트 : nc -l 1234 > foo.txt
// 파일을 보내려는 호스트 : nc 1.2.3.4 1234 < foo.txt
int msg_send(char *filename, char* relay_ip, int relay_port){  // [TODO] 만약에 릴레이대상 ip, port가 없어? 그러면 걍 파일삭제... 
	char command[100];
	sprintf(command, "nc %s %d < %s", relay_ip, relay_port, filename); 
	printf("%s\n",command);
	system(command);
	return 1;
}





int main(){
	//
}


