int msg_read2shell(char *filename){
	char command[100];
	sprintf(command, "cat %s", filename);
	system(command);
}

// Usage : char *relay_ip, *relay_port;msg_append_relayinfo("foo.txt", "127.0.0.1", 44444);
int msg_append_relayinfo(char *filename, char *relay_ip, int relay_port){
	/* 
	[파일 형식]
	relay ip
	relay port
	message 내용
	*/
	char command[100];
	
	// 첫줄에 라인 추가 : sed -i '1itask goes here' lll.txt
	sprintf(command, "sed -i '1i%d' %s", relay_port ,filename); 
	system(command);
	sprintf(command, "sed -i '1i%s' %s", relay_ip ,filename); 
	system(command);
}


// Usage :  char *relay_ip, *relay_port; 를 먼저 선언후에...
//          msg_extract_relayinfo("foo.txt",&relay_ip,&relay_port) --> Call by reference로 읽어온다
int msg_extract_relayinfo(char *filename, char **relay_ip, char **relay_port){
	/* 
	[파일 형식]
	relay ip
	relay port
	message 내용
	*/
    FILE * fp;
    size_t len = 0;
	char command[100];
    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
	getline(relay_ip, &len, fp);   // 파일첫줄에서 IP읽어서 relay_ip에저장
	getline(relay_port, &len, fp); // 파일두번째줄에서 port읽어서 relay_port에저장

	// file에서 상위 두줄삭제 : sed -i '1d' aaa.txt
	sprintf(command, "sed -i '1d' %s", filename);
	printf("%s",command);
	system(command);
	system(command);
	
    fclose(fp);
}


// 스레드 2개 !! - 받는얘가 백그라운드 스레드!


// 파일을 받으려는 호스트 : nc -l 1234 > foo.txt
// 파일을 보내려는 호스트 : nc 1.2.3.4 1234 < foo.txt
int msgsend(char *filename, char* relay_ip, int relay_port){ 
	char command[100];
	sprintf(command, "nc %s %d < %s", relay_ip, relay_port, filename); 
	printf("%s\n",command);
	system(command);
}
