#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>

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
	char cmd[256];
	snprintf(cmd, 256, "wget --spider --timeout=1 %s%s.pub 2>/dev/null", url, githubId);
	if(!system(cmd)) return 1; //valid
	return 0;
}

// Check whether [PGP_passphrase] string is  match with PGP private key(which is registered in the machine) or not.
int  auth_passphrase(char *PGP_passphrase, char *mygithubId){
    char cmd[256];
    char *pub_key ;
	int auth;
	
	if(!to_exist_publickey(mygithubId)) {
		printf("error! cannot get %s.pub...\n",mygithubId); 
	}
	register_pubkey(mygithubId);
	pub_key =(char*)get_pubkeyID(mygithubId); 
	if(!pub_key) return 0;
	
	system("gpg --help > auth_passphrase.tmp");
    snprintf(cmd, 256, "sudo gpg -r %s --encrypt auth_passphrase.tmp", pub_key); system(cmd);
    snprintf(cmd, 256, "echo %s | sudo gpg --passphrase-fd 0 -r %s --decrypt auth_passphrase.tmp.gpg > authresult.tmp",PGP_passphrase, pub_key); system(cmd);
    system("sudo rm auth_passphrase.tmp.gpg");
    system("rm auth_passphrase.tmp");

    FILE *f;
    f = fopen("authresult.tmp", "r");
    if(fgetc(f) == EOF) auth=0;
    else auth=1; //valid
	
	system("rm authresult.tmp");
	return auth;
}

// also register
int to_exist_publickey(char *githubId){
    char *url = "https://raw.githubusercontent.com/KAIST-IS521/2018-Spring/master/IndividualKeys/";
	char cmd[256];
	char githubIdpub[100];
	int ret;
    
    snprintf(githubIdpub, 256,"%s.pub", githubId);
	ret = access(githubIdpub, F_OK);
	if(ret==0) { // 0 : exist
		snprintf(cmd, 256, "gpg --import %s.pub 2>/dev/null", githubId); system(cmd);
		return 1;
	}
	else{ 
		if(is_valid_githubid(githubId)){ // no exist and download.
			snprintf(cmd, 256, "wget %s%s.pub 2>/dev/null", url, githubId); system(cmd);
			snprintf(cmd, 256, "gpg --import %s.pub 2>/dev/null", githubId); system(cmd);
			return 1;
		}
	}
	return 0; // no exist both local and server. 
}

int register_pubkey(char *githubId){
    char cmd[256];
	to_exist_publickey(githubId); // prerequisite : [githubId.pub] file. 
    snprintf(cmd, 256,  "gpg --import %s.pub 2>/dev/null", githubId); system(cmd);
	return 1;
}

int register_private_key(char *privfile){
	char cmd[256];
	snprintf(cmd, 256, "gpg --allow-secret-key-import --import %s 2>/dev/null", privfile); system(cmd);
	return 1;
}

char *get_pubkeyID(char *githubId){
    char cmd[256];
	int i;
	FILE *f;
	char c;
	char *pubkeyID = (char*)malloc(16);
	
	if (!to_exist_publickey(githubId)) return NULL; // prerequisite : [githubId.pub] file. 
    snprintf(cmd, 256, "sudo gpg %s.pub > KeyId.tmp 2>/dev/null",githubId); system(cmd);
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
	snprintf(cmd,256,"rm KeyId.tmp"); system(cmd);
	return pubkeyID;
}

int auth_user(char *githubId){ 
    char cmd[256];
	int i;
	FILE *f;
	char a[9],c;
	char *pubkeyID;
	to_exist_publickey(githubId);// prerequisite : [githubId.pub] file. 
	pubkeyID=get_pubkeyID(githubId);
	if(!pubkeyID) return 0;
    snprintf(cmd, 256, "sudo gpg --export-secret-keys -a %s > privateKey.tmp", pubkeyID);
    system(cmd);
    f = fopen("privateKey.tmp","r+");
    if(getc(f) == EOF) return 0;
    else{
        snprintf(cmd,256,"sudo rm privateKey.tmp");system(cmd);
        return 1;
    }
}


