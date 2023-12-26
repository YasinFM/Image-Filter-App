#include<stdio.h>
#include<conio.h>
#include <string.h>
#include <stdlib.h>

#define namelen 20

int b64decode(char input[250],char name[10]) {
    int i = 0;
    char c;
    char ch[15];
    FILE *fs;
    fs = fopen(input, "r");
    for (;i < 10;) {
        c = fgetc(fs);
        ch[i++]=c;
    }
    ch[i++]='\0';
    char command[200],e[10];
    char *found_string1 = strstr(ch, "Qk");
    char *found_string2 = strstr(ch, "/9j");
    if(found_string1 != NULL){
        char e[namelen] = "emailoutput.bmp\0";
        sprintf(command,"certutil -decode %s %s",name,e);
        //bmp
    }else if(found_string2 != NULL){
        char e[namelen] = "emailoutput.jpg\0";
        sprintf(command,"certutil -decode %s %s",name,e);
        //jpg
    }else{
        char e[namelen] = "emailoutput.png\0";
        sprintf(command,"certutil -decode %s %s",name,e);
        //png
    }
    system(command);
    return 0;
}