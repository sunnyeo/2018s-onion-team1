#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define RECV_H 25
#define RECV_W 60
#define RECV_Y 1
#define RECV_X 2

#define SEND_H 5
#define SEND_W 85
#define SEND_Y 26
#define SEND_X 2

#define LIST_H 25
#define LIST_W 24
#define LIST_Y 1
#define LIST_X 63
#define prompt "DVONION>"

#define DBSERVER_IP "127.0.0.1"
#define DBSERVER_PORT 4000

#define TMPFILE "test_tmpfile"
void trim(char* s){
    while(s[strlen(s)-1]=='\n' || s[strlen(s)-1]==' '){
        s[strlen(s)-1]=0;
    }
}
unsigned int fsize(const char *filename) {
    struct stat st; 
    if (stat(filename, &st) == 0)
        return st.st_size;
    return -1;
}
int main(){
    int n=0;
    FILE* fp=0;
    unsigned int size=0;
    char ip[32];
    char port[16];
    char gitid[64];
    char fname[128];
    char buf[256];
    char* tmpmsg = malloc(512);
    while(1){

        snprintf(buf, 256, "nc -l -p %d -v > %s", 4444, TMPFILE); // 이미 TMPFILE을 다운받은 상태임.
        system(buf);
        // decrypt the file!...

        if(fsize(TMPFILE) < 6){ 
            // sleep(1); 
            printf("[DBG] DBServer pocked me! I answered I'm still alive\n");
			snprintf(buf, 256, "rm %s", TMPFILE);
			system(buf);
            // exit(99);
			continue;
        }

        fp = fopen(TMPFILE, "r");
        if(fp==0){
            sleep(1);
            exit(100);
        }

        // check magic
        fgets(buf, 256, fp); trim(buf);
        if(!strcmp(buf, "final")){

            // check type
            fgets(buf, 256, fp); trim(buf);
            if(!strcmp(buf, "text")){
                fgets(gitid, 64, fp); trim(gitid); // sender ID
                fgets(fname, 128, fp); trim(fname);  // ignore
                fgets(buf, 256, fp); trim(buf);
                snprintf(tmpmsg, 512, "%s says: %s", gitid, buf);
				printf("%s\n", tmpmsg);
            }
            else if(!strcmp(buf, "file")){
                fgets(gitid, 64, fp); trim(gitid); // sender ID
                fgets(fname, 128, fp); trim(fname);  // ignore
                fclose(fp);

                // remove 4 lines
                snprintf(buf, 256, "sed -i '1d' %s", TMPFILE);
                system(buf);
                system(buf);
                system(buf);
                system(buf);

                snprintf(buf, 256, "mv " TMPFILE " %s", fname);
                system(buf);
                snprintf(tmpmsg, 512, "%s sent you %s file", gitid, fname);
				printf("%s\n", tmpmsg);
            }
            else{
                snprintf(tmpmsg, 512, "corrupted format");
            }
            continue;
        }

        // otherwise relay this to other peer.
        else{
            // buf was ip. second line is port.
            strncpy(ip, buf, 32); trim(ip);
            fgets(port, 16, fp); trim(port);
            fclose(fp);

            // remove 2 lines
            snprintf(buf, 256, "sed -i '1d' %s", TMPFILE);
            system(buf);
            system(buf);

            // relay the file.
            snprintf(buf, 256, "cat " TMPFILE " | nc %s %s", ip, port);
            system(buf);

            // for debug (remove later)
            snprintf(buf, 256, "relay to %s %s", ip, port);
			printf("%s\n", tmpmsg);
			
            continue;
        }

        // error!
        printf("protocol error!\n");
    }
}