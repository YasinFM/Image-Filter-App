#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#include <stdlib.h>
#include "fetch.h"


#pragma comment(lib, "ws2_32.lib")

void extractimageencode(const char* text , const char * outputimagefile ){
    const char* start = strstr(text , "\n\n");
    if ( start == NULL){
        printf("can't find start!!");//?
        return;
    }
    start += strlen("\n\n");
    //const char * end = strstr(start , "\0");
    size_t length = strlen(start);
    char * textencode = (char *)malloc(length + 1);
    strncpy(textencode , start , length);
    textencode[length] = '\0';

    FILE * outputfile = fopen(outputimagefile , "w");
    if( outputfile == NULL ){
        printf("failed to open the output file \n");
        return;
    }

    fprintf(outputfile , "%s" , textencode);
    fclose(outputfile);

    printf("text successfully added to '%s'. \n" , outputimagefile);
    free(textencode);

}


int main() {
    WSADATA wsaData;
    SOCKET sock;
    struct addrinfo hints, *res;
    char request[1024];
    int bytesReceived, totalBytesReceived;
    FILE *fp;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Error creating socket\n");
        WSACleanup();
        return 1;
    }

    // Resolve URL to IP address
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo("www.cs.sjsu.edu", "80", &hints, &res) != 0) {
        printf("Error resolving URL\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Connect to server
    if (connect(sock, res->ai_addr, res->ai_addrlen) == SOCKET_ERROR) {
        printf("Error connecting to server\n");
        freeaddrinfo(res);
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Send HTTP GET request
    sprintf(request, "GET /~pearce/modules/lectures/web/html/HTTP_files/image001.jpg HTTP/1.1\r\nHost:www.cs.sjsu.edu\r\n\r\n");
    send(sock, request, strlen(request), 0);

    // Receive response
    totalBytesReceived = 0;
    fp = fopen("./temp.txt", "wb");
    while ((bytesReceived = recv(sock, request, sizeof(request), 0)) > 0) {
        fwrite(request, 1, bytesReceived, fp);
        totalBytesReceived += bytesReceived;
        printf("%c",bytesReceived);
    }


    // Clean up
    freeaddrinfo(res);
    closesocket(sock);
    WSACleanup();
    fclose(fp);

    printf("Downloaded %d bytes\n", totalBytesReceived);



//    FILE *s;
//    s = fopen(".\\temp.txt","r");
//    const char *image = "C:\\Users\\parca\\Desktop\\networkingphotoshopproject\\imageencode.txt";
//
//    fseek(s , 0 , SEEK_END);
//    long file_size = ftell(s);
//    rewind(s);
//
//    char * filetmp = (char * ) malloc(file_size + 1);
//    fread(filetmp , sizeof(char) , file_size , s);
//
//    filetmp[file_size] = '\0';
//
//    fclose(s);
//    extractimageencode(filetmp , image);
//
//    free(filetmp);
//
//
    fetch("temp.txt","./imageencoded.txt");
    return 0;
}