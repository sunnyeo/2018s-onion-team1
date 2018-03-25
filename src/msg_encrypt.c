#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h> /* For htonl() */


// 그럼 키를 register 한 상태에서, 그 상대방의 키를 이용해서 
// 로컬에 있는 testfile.txt를 암호화하는 함수 
// 로컬에서 testfile.txt를 암호화해서 testfile_encrypt.txt 파일을 만드는 함수
int msgfile_encrypt(char *file_name, char *githubId){
    
    char cmd[100];
    char cmd_result[30];
    char key_temp[30];
    char *temp;
    char *key;
    FILE *import;

    snprintf(cmd, 100, "gpg --import %s.pub 2>&1", githubId);
    import = popen(cmd, "r");

    if(import == NULL)
    {
        printf("There is no %s.pub file\n", githubId);
        return -1;
    }
    fgets(cmd_result, 30, import);
    temp = strstr(cmd_result, "key");
    key = strstr(temp," ");
    temp = strstr(key, ":");
    *temp = NULL;
    key=strncpy(key_temp, key+1, 15);

    pclose(import);

    snprintf(cmd, 100, "gpg --armor --encrypt --recipient %s %s", key, file_name);

    system(cmd);

    return 0;
}


