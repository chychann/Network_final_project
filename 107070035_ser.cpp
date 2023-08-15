#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <winsock2.h>
#include <time.h>
#include <signal.h>
#include <Windows.h>
#include <unistd.h>

void game_loop(int fd){
    printf("===========================\n");
    printf("Game Start\n");
    printf("===========================\n");
    

    srand(time(NULL));
    int random_number = (rand() % 99)+1;
    //printf("%d",random_number);
    char buff[256]={};
    int converted_number;
    char word='0';
    word='S';
    send(fd, &word,sizeof(word),0);
    while (1) {
        sleep(1);
        if (recv(fd,buff,sizeof(buff),0)==-1){
            printf("error at recv.\n");
        }
        //printf("%s ",buff);
        converted_number = atoi(buff);
        //printf("%d\n",converted_number);
        if (converted_number == random_number) {
            word='=';
            //printf("=\n");
            printf("Correct\n");
            send(fd,&word,sizeof(word),0);
            return;
        }else if (converted_number > random_number) {
            word='>';
            //printf(">\n");
            send(fd,&word,sizeof(word),0);
        }else if (converted_number < random_number) {
            word='<';
            //printf("<\n");
            send(fd,&word,sizeof(word),0);
        }else {
            printf("error at guess\n");
            exit(1);
        }
    }
}

int main(int argc, char *argv[])
{
    WSADATA mywsadata; //your wsadata struct, it will be filled by WSAStartup
    WSAStartup(0x0202,&mywsadata);
    //printf("success0.\n");
    int port_num=atoi(argv[2]);
    printf("%d\n",port_num);

    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    char sendBuff[1025];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if (listenfd == -1){
        printf("Fail to create a socket.\n");
        closesocket(listenfd);
        //exit(1);
    }
    //printf("success1.\n");
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_num);
    //printf("success2.\n");
    if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1){
        printf("Fail to bind a socket.\n");
        closesocket(listenfd);
        exit(1);
    }
    listen(listenfd, 10);
    //printf("success3.\n");

    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        if (connfd == -1){
            printf("error at connection.\n");
            exit(1);
        }
        //printf("new round\n");
        //printf("accept success\n");
        /*char word='S';
        send(connfd, &word,sizeof(word),0);*/
        game_loop(connfd);
        printf("Next Round\n");
        close(connfd);
    }


    WSACleanup();
}
