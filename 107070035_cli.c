#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <winsock2.h>
#include <time.h>
#include <signal.h>
#include <Windows.h>
#include <unistd.h>
#define port 80

int main(int argc, char *argv[]){

    WSADATA mywsadata; //your wsadata struct, it will be filled by WSAStartup
    WSAStartup(0x0202,&mywsadata);

    int sockfd=0;
    char buff[256]={};
    char receive='0';
    //char port_num[]={};
    int port_num=atoi(argv[3]);
    // int len = strlen(argv[2]);
    // printf("%s\n",argv[2]);
    // for(int i =0;i<len;i++){
    //     port_num[i]=argv[2][1];
    // }
    // printf("%s\n",port_num);
    //char r_s='0';
    while(1){
        sockfd=socket(AF_INET , SOCK_STREAM , 0);
        //listenfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1){
            printf("Fail to create a socket.\n");
        }

        struct sockaddr_in serv_addr;
        memset(&serv_addr, '0', sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port_num);
        serv_addr.sin_addr.s_addr = inet_addr(argv[2]);


        int err= connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        if (err ==-1){
            printf("connection error!\n");
        }else{
            printf("Connection Success.\n");
        }

        int game_start=1;


        recv(sockfd,&receive,1,0);
        //printf("%c",receive);
        if(receive!='S'){
            printf("%c",receive);
            printf("Error at Start message.\n");
            game_start=0;
            exit(1);
        }
        //int guessed_number, converted_number;
        //recv(sockfd,&receive,1,0);
        int smallnumber=1,bignumber=100;
        while(game_start){
            printf("----------------------------\n\n");
            printf("Guess a number:\n");
            scanf("%s",&buff);
            //printf("%s",buff);
            //converted_number=htonl(guessed_number);
            send(sockfd,buff,sizeof(buff),0);
            //int converted_number = htonl(guessed_number);
            //send(sockfd, &converted_number, sizeof(uint32_t),0);

            recv(sockfd,&receive,1,0);
            if(receive=='='){
                printf("Answer Correct!\n\n");
                printf("===========================\n");
                closesocket(sockfd);
                //receive='S';
                sleep(1);
                break;
            }else if(receive=='>'){
                int tmp=atoi(buff);
                if(tmp<=bignumber) bignumber=tmp;
                printf("Lower than %d\nHiger than %d\n", bignumber,smallnumber);
            }else if(receive=='<'){
                int tmp=atoi(buff);
                if(tmp>smallnumber) smallnumber=tmp;
                printf("Lower than %d\nHiger than %d\n", bignumber,smallnumber);
            }else{
                printf("Error at receive.\n");
                closesocket(sockfd);
                exit(1);
            }
        }
    }
    WSACleanup();
}
