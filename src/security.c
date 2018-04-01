#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *escapeshell(char* str){// w3challs safels
	/*
	char * buffer = (char *) malloc(strlen(str)+1);
	strcpy(buffer,str);
	return buffer;
	*/
	
		char cmd[256];
        char dangerous[] = "";//"#&;`'\"|*?~<>^()[]{}$\\,\x0a";
        unsigned int i = 0, j = 0, count = 0;
        char * buffer = (char *) malloc(strlen(str)*2 + 1);
		memset(buffer, 0, strlen(str)*2 + 1);

        while( i<strlen(str) && count<strlen(str) ){
                for( j=0; j<strlen(dangerous); j++ ){
                        if( str[i] == dangerous[j] ){
                                buffer[count++] = '\\';
                                break;
                        }
                }
                buffer[count++] = str[i++];
        }
	snprintf(cmd, 256, "echo %s %s >> log", str, buffer); system(cmd);
	return buffer;
 
}
/*
int main(int argc, char *argv[]){
	printf("%s\n",escapeshell(argv[1]));
}
*/