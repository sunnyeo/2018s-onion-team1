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


// [TODO] public key들이 저장되는 tmp폴더를 따로 만들기. 



// download [githubId.pub] from github repository.
// developer : hansh09
int download_pubkey(char *githubId){
        char *url = "https://raw.githubusercontent.com/KAIST-IS521/2018-Spring/master/IndividualKeys/";
		char cmd[BUFF_SIZE];
		char githubIdpub[100];
		int ret;
        snprintf(cmd, BUFF_SIZE, "wget %s%s.pub 2>/dev/null", url, githubId);
        snprintf(githubIdpub, BUFF_SIZE,"%s.pub", githubId);
		ret = access(githubIdpub, F_OK);
		if(ret) // 존재하지 않는다면 다운받는다
		{
			system(cmd);
		}
	return 0;	
}


// register [githubId.pub]  
// developer : MincheolSon
int register_pubkey(char *githubId){
    char cmd[BUFF_SIZE];
	// prerequisite : [githubId.pub] file. 
	download_pubkey(githubId);
    snprintf(cmd, BUFF_SIZE,  "gpg --import %s.pub 2>/dev/null", githubId);
    system(cmd);
}


// register [private key]
// developer : hansh09
int register_private_key(char *privfile){
	char cmd[BUFF_SIZE];
	snprintf(cmd, BUFF_SIZE, "gpg --allow-secret-key-import --import %s 2>/dev/null", privfile);
	system(cmd);
}


// get key ID of [githubId.pub] 
// developer : Dauren
char *get_pubkeyID(char *githubId){
    char cmd[BUFF_SIZE];
	int i;
	FILE *f;
	char c;
	char *pubkey_id = (char*)malloc(10);
	
	// prerequisite : [githubId.pub] file. 
	download_pubkey(githubId);
    snprintf(cmd, BUFF_SIZE, "sudo gpg %s.pub > KeyId.tmp 2>/dev/null",githubId);
    //create the file containing the KeyId of user githubId
    system(cmd);
    f = fopen("KeyId.tmp", "r");
    

    if(fgetc(f) == EOF) //if file is empty it means that user is not registered
    {
        printf("User %s is not registered yet\n", githubId);
        return 0;
    }
    // get the user's KeyId from KeyId.tmp file
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
	snprintf(cmd,BUFF_SIZE,"rm KeyId.tmp");
	system(cmd);
	return pubkey_id;
}


// verify local machine is githubId's or not.
// deveopler : Dauren
int auth_user(char *githubId){ 
    char cmd[BUFF_SIZE];
	int i;
	FILE *f;
	char a[9],c;

	// prerequisite : [githubId.pub] file. 
	download_pubkey(githubId);
	
    // now key id of user is stored in *a
    snprintf(cmd, BUFF_SIZE, "sudo gpg --export-secret-keys -a %s > privateKey.tmp",get_pubkeyID(githubId));

    // check if the private key of user is stored on this machine
    // If it is, then it will be saved in the privateKey.tmp. Otherwise,
    // this command will give warning and privateKey.tmp will be empty
    system(cmd);
    f = fopen("privateKey.tmp","r");
    if(getc(f) == EOF)
        return 0;
    else
    {
        snprintf(cmd,BUFF_SIZE,"sudo rm privateKey.tmp"); //delete the privateKey file
                                                          //before exiting this function
        system(cmd);
        return 1;
    }
}


// [TODO] 잘됨. 하지만 쉘에 메시지가 뜸. 어차피 백그라운드로 돌아갈거라 상관없나...
// Check whether [PGP_passphrase] string is 
// match with PGP private key(which is registered in the machine) or not.
// (If you need mygithubId, you can use it.)
int  auth_passphrase(char *PGP_passphrase, char *mygithubId)
{
    char cmd[BUFF_SIZE];
    char *pub_key = get_pubkeyID(mygithubId);
    // first create some random file for encryption
    system("gpg --help > dummyfile.tmp");
    // then encrypt this file with mygithubId's public key
    snprintf(cmd, BUFF_SIZE, "sudo gpg -r %s --encrypt dummyfile.tmp 2>/dev/null", pub_key);
    system(cmd);
    // then try to decrypt the encrypted file with mygithubId's private key
    // in order to do this we must pass mygithubId's passphrase
    // If this passphrase is correct resultant authresult.tmp will be identical to
    // the dummyfile.tmp, otherwise authresult.tmp will be empty
    snprintf(cmd, BUFF_SIZE, "echo %s | sudo gpg --passphrase-fd 0 -r %s --decrypt dummyfile.tmp.gpg > authresult.tmp 2>/dev/null",PGP_passphrase, pub_key);
    system(cmd);
    // delete all of the created files to not the waste the memory
    system("sudo rm dummyfile.tmp.gpg");
    system("rm dummyfile.tmp");

    FILE *f;
    f = fopen("authresult.tmp", "r");
    if(fgetc(f) == EOF)
        return 0;
    else
        return 1;
}


