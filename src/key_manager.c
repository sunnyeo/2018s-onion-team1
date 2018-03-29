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

int is_valid_githubid(char *githubId);
int auth_passphrase(char *PGP_passphrase, char *mygithubId);
int to_exist_publickey(char *githubId);
int register_pubkey(char *githubId);
int register_private_key(char *privfile);
char *get_pubkeyID(char *githubId);
int auth_user(char *githubId);
int onion_register_all_pubkey();

int is_valid_githubid(char *githubId){
	char *url = "https://github.com/KAIST-IS521/2018-Spring/blob/master/IndividualKeys/";
	char cmd[BUFF_SIZE];
	snprintf(cmd, BUFF_SIZE, "wget --spider --timeout=1 %s%s.pub 2>/dev/null", url, githubId);
	if(!system(cmd)) return 1; //valid
	return 0;
}

// Check whether [PGP_passphrase] string is  match with PGP private key(which is registered in the machine) or not.
int  auth_passphrase(char *PGP_passphrase, char *mygithubId){
    char cmd[BUFF_SIZE];
    char *pub_key ;
	int auth;
	
	if(!to_exist_publickey(mygithubId)) {
		printf("error! cannot get %s.pub...\n",mygithubId); 
	}
	register_pubkey(mygithubId);
	pub_key =(char*)get_pubkeyID(mygithubId); 
	if(!pub_key) return 0;
	
	system("gpg --help > auth_passphrase.tmp");
    snprintf(cmd, BUFF_SIZE, "sudo gpg -r %s --encrypt auth_passphrase.tmp", pub_key);
    system(cmd);
    snprintf(cmd, BUFF_SIZE, "echo %s | sudo gpg --passphrase-fd 0 -r %s --decrypt auth_passphrase.tmp.gpg > authresult.tmp",PGP_passphrase, pub_key);
    system(cmd);
    system("sudo rm auth_passphrase.tmp.gpg");
    system("rm auth_passphrase.tmp");

    FILE *f;
    f = fopen("authresult.tmp", "r");
    if(fgetc(f) == EOF) auth=0;
    else auth=1; //valid
	
	system("rm authresult.tmp");
	return auth;
}

int to_exist_publickey(char *githubId){
    char *url = "https://raw.githubusercontent.com/KAIST-IS521/2018-Spring/master/IndividualKeys/";
	char cmd[BUFF_SIZE];
	char githubIdpub[100];
	int ret;
    
    snprintf(githubIdpub, BUFF_SIZE,"%s.pub", githubId);
	ret = access(githubIdpub, F_OK);
	if(ret==0) return 1;
	else{ // no exist
		if(is_valid_githubid(githubId)){
			snprintf(cmd, BUFF_SIZE, "wget %s%s.pub 2>/dev/null", url, githubId);
			system(cmd);
			return 1;
		}
	}
	return 0; // no exist both local and server. 
}

int register_pubkey(char *githubId){
    char cmd[BUFF_SIZE];
	to_exist_publickey(githubId); // prerequisite : [githubId.pub] file. 
    snprintf(cmd, BUFF_SIZE,  "gpg --import %s.pub 2>/dev/null", githubId);
    system(cmd);
}

int register_private_key(char *privfile){
	char cmd[BUFF_SIZE];
	snprintf(cmd, BUFF_SIZE, "gpg --allow-secret-key-import --import %s 2>/dev/null", privfile);
	system(cmd);
}

char *get_pubkeyID(char *githubId){
    char cmd[BUFF_SIZE];
	int i;
	FILE *f;
	char c;
	char *pubkeyID = (char*)malloc(16);
	if (!to_exist_publickey(githubId)) return NULL; // prerequisite : [githubId.pub] file. 
    snprintf(cmd, BUFF_SIZE, "sudo gpg %s.pub > KeyId.tmp 2>/dev/null",githubId);
    system(cmd);
    f = fopen("KeyId.tmp", "r");
    if(fgetc(f) == EOF) return NULL;
    for(i=0;;c = fgetc(f))
    {
        if(c == '/')
        {
            while((c=fgetc(f)) != ' ')
                pubkeyID[i++] = c;
            pubkeyID[i] = '\0';
            break;
        }
    }
	snprintf(cmd,BUFF_SIZE,"rm KeyId.tmp");
	system(cmd);
	return pubkeyID;
}

int auth_user(char *githubId){ 
    char cmd[BUFF_SIZE];
	int i;
	FILE *f;
	char a[9],c;
	char *pubkeyID;
	to_exist_publickey(githubId);// prerequisite : [githubId.pub] file. 
	pubkeyID=get_pubkeyID(githubId);
	if(!pubkeyID) return 0;
    snprintf(cmd, BUFF_SIZE, "sudo gpg --export-secret-keys -a %s > privateKey.tmp", pubkeyID);
    system(cmd);
    f = fopen("privateKey.tmp","r+");
    if(getc(f) == EOF) return 0;
    else{
        snprintf(cmd,BUFF_SIZE,"sudo rm privateKey.tmp"); 
        system(cmd);
        return 1;
    }
}

int onion_register_all_pubkey(){
	FILE *fp;
	char line[100];
	char *githubID;
	fp = fopen("OnionUser.db.tmp", "r"); 
	int i=0;
	
	if (fp == NULL) {
		exit(101);
	}
	else{
		while(!feof(fp)){
			fgets(line, 100, fp);
			if(line[0]=='-') break; // [TODO] trim
			githubID = strchr(line, ' ')+1;
			githubID = strchr(githubID, ' ')+1;
			githubID[strlen(githubID)-1] = '\0';
			printf("%s\n",githubID);
			
			to_exist_publickey(githubID);
			register_pubkey(githubID);
		}
		printf("key registered\n");
	}
	return 0;
}


