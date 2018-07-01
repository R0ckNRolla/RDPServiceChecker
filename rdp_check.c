#include <winsock2.h>
#include <winsock.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library

char packet_bytes[] = {
  0x03, 0x00, 0x00, 0x2f, 0x2a, 0xe0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x43, 0x6f, 0x6f, 0x6b, 0x69,
  0x65, 0x3a, 0x20, 0x6d, 0x73, 0x74, 0x73, 0x68,
  0x61, 0x73, 0x68, 0x3d, 0x46, 0x54, 0x42, 0x43,
  0x4f, 0x5c, 0x41, 0x37, 0x30, 0x0d, 0x0a, 0x01,
  0x00, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00
};






int check_rdp(char ip[],int port)
{
    ///returns:
    ///0 for open rdp
    ///1 for Errors while sending or receiving data or making socket
    ///2 for closed rdp



    char reply[5];
    SOCKADDR_IN address;
    SOCKET sock;
    if(!(sock=socket(AF_INET,SOCK_STREAM,0))){return 1;}

    //printf("Socket Created\n");

    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);
    address.sin_family = AF_INET;

    if((connect(sock,(const struct sockaddr *)&address,sizeof(address)))<0){return 2;}


    char *string = packet_bytes;

    if (( send( sock , string , 47 ,0) ) <0){return 1;}


    //printf("Data Sent\n");
    int size=0;
    if((size = recv(sock,reply,4,0) == SOCKET_ERROR)){return 1;}
    reply[4] = '\0';
    if ( reply[0]==3 && reply[1]==0 && reply[2]==0){return 0;}
    else {return 2;}



    return 0;


}







int main(int argc, char *argv[])
{
    if(argc!=2){printf("USAGE: rdp.exe <ip_list_file>");return 1;}


    WSADATA wsa;
    char *ip; //pointer for token
    char ip_string[256];  //char to pass to func



    int stat;
    FILE *in,*out;


    char ipad[40];  //ip with port
    char ipad2[40];

    char port_str[6];
    int port;


    char temp;


    //scanf("%s",ip);
    in = fopen(argv[1],"r");
    out = fopen("OpenRDP.txt","w");
    if(!in){printf("Couldn't Open File");return 1;}
    if(WSAStartup(MAKEWORD(2,2),&wsa)){
        printf("Error");
        return 1;
    }
    int string_counter=0;
    while(!feof(in)){
        string_counter=0;
        temp = getc(in);
        while(temp!='\n' && !feof(in))
        {
            ipad[string_counter]=temp;
            string_counter++;
            temp = getc(in);
        }
        ipad[string_counter]='\0';
        strcpy(ipad2,ipad);
        ip = strtok(ipad,":");
        port = (int)atoi(strtok(NULL,":"));

        strcpy(ip_string,ip);

        stat=check_rdp(ip_string,port);
        
        if(stat==1) printf("Problem While Making Socket\nRetry With Administrators Permisions and Check your Internet Connection");
        if(stat==2) printf("RDP is Not Running on  %s:%d",ip,port);
        if(stat==0)
        {
            printf("RDP is Running on  %s:%d",ip,port);
            fputs(ipad2,out);
            
        }
        
        
        
        printf("\n");

}

    return 0;
}
