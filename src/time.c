#include <stdio.h>
#include <time.h>

int get_time(char *buff, size_t buff_size)
{
    struct tm *t;
    time_t timer;

    time(&timer);
    t = localtime(&timer);

    snprintf(buff, buff_size, "%02d-%02d %02d:%02d",
            t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min);

    return 1;
}

int main()
{
    char time_buff[80];

    get_time(time_buff, sizeof(time_buff));
    printf("%s\n", time_buff);
    
    return 0;
}
