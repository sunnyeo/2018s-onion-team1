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

// 서버로부터 [githubId.pub]를 다운받는다.
// developer : hansh09
int download_pubkey(char *githubId){
        char *url = "https://raw.githubusercontent.com/KAIST-IS521/2018-Spring/master/IndividualKeys/";
		char cmd[BUFF_SIZE];
		char githubIdpub[100];
		int ret;
        snprintf(cmd, BUFF_SIZE, "wget %s%s.pub", url, githubId);
        snprintf(githubIdpub, BUFF_SIZE,"%s.pub", githubId);
		ret = access(githubIdpub, F_OK);
		if(ret) // 존재하지 않는다면 다운받는다
		{
			printf("%d",ret);
			system(cmd);
		}
	return 0;	
}


// 로컬에 현재 [githubId.pub]에 있는 상태에서, 그 키를 등록한다. 
// developer : MincheolSon
int register_pubkey(char *githubId){
	
    char cmd[BUFF_SIZE];
    snprintf(cmd, BUFF_SIZE,  "gpg --import %s.pub", githubId);
    system(cmd);
}


// 로컬에 현재 [private key]가 있는 상태에서, 그 키를 등록한다.
// developer : hansh09
int register_private_key(char *privfile){
	char cmd[BUFF_SIZE];

	snprintf(cmd, BUFF_SIZE, "gpg --allow-secret-key-import --import %s", privfile);
	system(cmd);
}


// 로컬에 [사용자이름].pub가 있을 때 사용자의 key ID를 리턴한다.
// developer : Dauren
char *get_pubkey(char *githubID){

    char cmd[BUFF_SIZE];
	int i;
	FILE *f;
	char c;
	char *pubkey_id = (char*)malloc(9);
	
    snprintf(cmd, BUFF_SIZE, "sudo gpg %s\.pub > KeyId.txt",githubID);
    //create the file containing the KeyId of user githubId
    system(cmd);
    f = fopen("KeyId.txt", "r");
    

    if(fgetc(f) == EOF) //if file is empty it means that user is not registered
    {
        printf("User %s is not registered yet\n", githubID);
        return 0;
    }
    // get the user's KeyId from KeyId.txt file
    // file format is as following
    // pub  <size of key>/<key id> <creation date> <user name> <email>
	//      ex) pub  2048R/EDE8D438 2018-03-22 jiwon choi (Second gpg key....) <jiwon.choi@kaist.ac.kr>
    // we only need <key id> part

    for(i=0;;c = fgetc(f))
    {
        if(c == '/')
        {
            while((c=fgetc(f)) != ' ')
                pubkey_id[i++] = c;
            pubkey_id[i] = '\0';
            break;
        }
    }
	snprintf(cmd,1000,"sudo rm KeyId.txt");
	return pubkey_id;

}


// 현재 머신이 githubId의 것인지 검증한다.
// deveopler : Dauren
int auth_user(char *githubId){ 


    char cmd[BUFF_SIZE];
	int i;
	FILE *f;
	char a[9],c;

	// 사전에 반드시 자신의 public key도 로컬에 있어야 한다. 
	download_pubkey(githubId);
	
    // now key id of user is stored in *a
    snprintf(cmd, 1000, "sudo gpg --export-secret-keys -a %s > privateKey.txt",get_pubkey(githubId));

    // check if the private key of user is stored on this machine
    // If it is, then it will be saved in the privateKey.txt. Otherwise,
    // this command will give warning and privateKey.txt will be empty

    system(cmd);
    f = fopen("privateKey.txt","r");
    if(getc(f) == EOF)
        return 0;
    else
    {
        snprintf(cmd,1000,"sudo rm privateKey.txt"); //delete the privateKey file
                                                    //before exiting this function
        system(cmd);
        return 1;
    }
}


int main(int argc, char *argv[]){
	// 테스트 코드들... 곧 지울것임
	download_pubkey("eternalklaus");
	
	if(auth_user("eternalklaus"))
		printf("이 머신은 eternalklaus의 것임\n");
	printf("eternalklaus's public key %s\n",get_pubkey("eternalklaus"));
	//dbserver_sendcommand(argv[1]);
}