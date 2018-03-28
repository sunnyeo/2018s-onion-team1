// gcc -o dvonion dvonion.c -lncurses -lpthread
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

#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<fcntl.h>
#include 	<signal.h>
#include 	<sys/socket.h>
#include 	<sys/file.h>
#include 	<sys/utsname.h>
#include 	<netinet/in.h>
#include 	<time.h>

#include <netdb.h>
#include <ifaddrs.h>



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

typedef struct __record{
    unsigned int no;
    char* ip;
    char* port;
    char* id;
    struct __record* next;
}record, *precord;

typedef struct _MSGQ{
    struct _MSGQ* next;
    char* msg;
}MSGQ, *PMSGQ;

precord onion_route_msg(const char* from, const char* to, const char* msg);
void parse_db(const char* fname);
int dbserver_interact(char *command);
void trim(char* s);
void draw_server_window();
unsigned int fsize(const char *filename);
unsigned int g_port;
PMSGQ g_head;
char g_id[32];
char g_passphrase[256];
int lock=0;

void queue_msg(char* msg){
    PMSGQ tmp = (PMSGQ)malloc(sizeof(MSGQ));
    memset(tmp, 0, sizeof(MSGQ));
    tmp->msg = malloc(512);
    memset(tmp->msg, 0, 512);
    snprintf(tmp->msg, 512, "%s", msg);
    tmp->next = g_head;
    g_head = tmp;
    draw_server_window();
}

void draw_list_window(){
    int i=0;
    FILE* fp;
    unsigned int size;
    char* buf = malloc(256);
    char* name;
    int len;
    PMSGQ tmp=0;
    WINDOW* win = newwin(LIST_H, LIST_W, LIST_Y, LIST_X);
    if(!lock){
        dbserver_interact("@userlist");

        fp = fopen("OnionUser.db.tmp", "r");
        if(fp==0){
            sleep(1);
            exit(1002);
        }

        // check magic
        while( fgets(buf, 256, fp) ){
            trim(buf);
            if(buf[0]=='-') break;
            buf = strchr(buf, ' ') + 1;
            name = strchr(buf, ' ') + 1;   // second space.
            trim(name);
            lock++;
            mvwprintw(win, 1+i, 1, name);
            box(win, 0, 0);
            wrefresh(win);
            lock--;
            i++;
        }

        fclose(fp);
        sleep(1);
    }
}

void draw_server_window(){
    int i=0;
    PMSGQ tmp=0;
    WINDOW* win = newwin(RECV_H, RECV_W, RECV_Y, RECV_X);
    if(!lock){
        lock++;
        tmp = g_head;
        while(tmp != 0){
            mvwprintw(win, 1+i, 1, tmp->msg);
            
            i++;
            tmp = tmp->next;
            if(i > RECV_H-2) break;
        }
        box(win, 0, 0);
        wrefresh(win);
        lock--;
    }
}

unsigned int fsize(const char *filename) {
    struct stat st; 
    if (stat(filename, &st) == 0)
        return st.st_size;
    return -1;
}

void trim(char* s){
    while(s[strlen(s)-1]=='\n' || s[strlen(s)-1]==' '){
        s[strlen(s)-1]=0;
    }
}

int get_total_usernum(){
	FILE *fp;
	fp = fopen("OnionUser.db.tmp","r");
	int i=0;
	char line[100];
	if (fp == NULL) return 0;
	else{
		while(!feof(fp)){ 
			fgets(line, 100, fp);
			if(line[0]=='-') break; 
			i++;
		}
	}
	return i;
}

#define TMPFILE "tmptmp"
void* server_thread(void* param){

    PMSGQ tmp=0;
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

        snprintf(buf, 256, "nc -l -p %d > %s 2>/dev/null", g_port, TMPFILE);
        system(buf);
        // decrypt the file!...

        if(fsize(TMPFILE) < 6){
            sleep(1);
            endwin();
            printf("error!\n");
            exit(99);       
        }

        fp = fopen(TMPFILE, "r");
        if(fp==0){
            endwin();
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
				
				// [TODO][암호화] sed -d 4 line, verify it, and print it. 
                snprintf(tmpmsg, 512, "%s says: %s", gitid, buf);
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
            }
            else{
                snprintf(tmpmsg, 512, "corrupted format");
            }

            queue_msg(tmpmsg);
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
            queue_msg(buf);
            continue;
        }

        // error!
        queue_msg("protocol error!");
    }
}

void* list_thread(void* param){
    while(1){
        draw_list_window();
        sleep(1);
    }
}

void* listnum_thread(void *param){
	// [TODO]
	// snprintf(buf,256,"total %d users in onion network\n",size);
	// mvwprintw(win, ?, 1, buf);
}

char *get_hostip(char *interface){
	struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];
	char *buf = (char*)malloc(NI_MAXHOST);
	
    if (getifaddrs(&ifaddr) == -1){
		printf("ip error!");
        exit(18);
    }
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) 
    {
        if (ifa->ifa_addr == NULL)
            continue;  

        s=getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
        if((strcmp(ifa->ifa_name,interface)==0)&&(ifa->ifa_addr->sa_family==AF_INET)){
            if (s != 0){
				printf("ip rror!");
                exit(18);
            }
			strncpy(buf, host, sizeof(host));
        }
    }
    freeifaddrs(ifaddr);
	return buf;
}

char* get_input(WINDOW* win){
    char c;
    char* buf;
    buf = malloc(256);
    memset(buf, 0, 256);
    strcpy(buf, prompt);
    unsigned int i=strlen(prompt);

    while(c = mvwgetch(win, 1, 1+i)){
        if(c=='\n') break;
        if(c==127){         // ncurses back space ascii number: 127
            if(i==strlen(prompt)) continue;
            buf[--i] = ' ';
            if(!lock){
                lock++;
                box(win, 0, 0);
                mvwprintw(win, 1, 1, buf);
                wrefresh(win);
                lock--;
            }
            continue;
        }

        buf[i++] = c;
        if(!lock){
            lock++;
            box(win, 0, 0);
            mvwprintw(win, 1, 1, buf);
            wrefresh(win);
            lock--;
        }
        if(i>255){
            break;
        }
    }
    return buf;
}

#define BUFF_SIZE 4096
int dbserver_interact(char *command){
   int   client_socket;
   struct sockaddr_in   server_addr;
   char   buff[BUFF_SIZE+5];
   int    fd;

   client_socket  = socket( PF_INET, SOCK_STREAM, 0);
   if( -1 == client_socket){
      printf( "[DBSERVER ERROR] socket generation fail\n");
      exit( 1);
   }

   memset( &server_addr, 0, sizeof( server_addr));
   server_addr.sin_family     = AF_INET;
   server_addr.sin_port       = htons(DBSERVER_PORT);
   server_addr.sin_addr.s_addr= inet_addr(DBSERVER_IP);
   if( -1 == connect( client_socket, (struct sockaddr*)&server_addr, sizeof( server_addr) ) )
   {
      printf( "[DBSERVER ERROR] server connection fail\n");
      exit( 1);
   }
   
   write( client_socket, command, strlen(command)+1);
   read ( client_socket, buff, BUFF_SIZE);
   
   if(!strncmp(command,"@userlist",strlen("@userlist"))){
       fd=open("OnionUser.db.tmp", O_RDWR | O_TRUNC | O_CREAT, 0777);
       write(fd, buff, strlen(buff));
       close(fd);
   }
   close( client_socket);
   return 0;
}

// [ID] [MSG]
int dv_send(char* str, int isfile){
    char* msg = strchr(str, ' ')+1;
    char* to = str;
    char* from = g_id;
    char* p = strchr(to, ' ');
    p[0] = 0;

    parse_db("OnionUser.db.tmp");
    precord t = onion_route_msg(from, to, msg);  // [TODO][암호화]   
    printf("ip:%s, port%s\n", t->ip, t->port);

    char* cmd = malloc(256);
    snprintf(cmd, 256, "cat onion | nc %s %s", t->ip, t->port);
    system(cmd);

    snprintf(cmd, 256, "to %s: %s", to, msg);
    queue_msg(cmd);
}

void main(){
    char cmd[256];
    pthread_t th1=0;
    pthread_t th2=0;
	
    // init stuffs...
    g_head = 0;
    srand(time(0));
    g_port = 40000 + ((3133777*rand())%10000);
    printf("enter your github (your port is %d) ID: ", g_port);
    scanf("%32s", g_id);
	printf("enter your passphrase : ");
	scanf("%256s",g_passphrase);
	
	// [TODO] github validation is temporally disabled on demo. enable it~
	/*
	if(!is_valid_githubid(g_id)||!auth_passphrase(g_passphrase, g_id)){
		printf("login failed.\n");
		exit(1);
	}
	*/
	if(!auth_passphrase(g_passphrase, g_id)){
		printf("login failed.\n");
		exit(1);
	}
	
	char *g_ip = get_hostip("eth0");
    snprintf(cmd, 256, "@adduser %s %d %s", g_ip, g_port, g_id);
    dbserver_interact(cmd);
    dbserver_interact("@userlist");
    
    initscr();
    noecho();
    /* setup client window */
    WINDOW* win = newwin(SEND_H, SEND_W, SEND_Y, SEND_X);
    refresh();  // refresh entire screen

    pthread_create( &th1, 0, server_thread, (void*)win);
    pthread_detach( th1 );

    pthread_create( &th2, 0, list_thread, (void*)win);
    pthread_detach( th2 );

    // draw a box at (y,x)
    box(win, 0, 0);

    usleep(2000);
    char* cmd2;
    while(1){
        wclear(win);
        draw_server_window(win);
        if(!lock){
            lock++;
            box(win, 0, 0);
            mvwprintw(win, 1, 1, prompt);
            wrefresh(win);  // only refresh the window
            lock--;
        }
        cmd2 = get_input(win);
        if(!strcmp(cmd2, prompt "/quit")){
            endwin();
            system("pkill nc 2>/dev/null");
            system("rm OnionUser.db.tmp");
            snprintf(cmd, 256, "@deleteuser %s", g_id);
            dbserver_interact(cmd);
            _exit(0);
            return;
        }
        if(!strncmp(cmd2, prompt "/sendmsg", strlen(prompt "/sendmsg"))){
            dv_send(cmd2 + strlen(prompt "/sendmsg "), 0);
        }
        if(!strncmp(cmd2, prompt "/sendfile", strlen(prompt "/sendfile"))){
            dv_send(cmd2 + strlen(prompt "/sendfile "), 1);
        }

        free(cmd2);
    }

    endwin();
}

