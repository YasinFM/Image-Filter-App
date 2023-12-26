#include<stdio.h>
#include<conio.h>
#include <string.h>

int fetch(char oldt[200],char newt[200]) {
    FILE *fs, *fn;
    fs = fopen(oldt, "r");
    if(fs == NULL)
    {
        printf("\nError in Opening the first file, %s", oldt);
        getch();
        return 0;
    }
    fn = fopen(newt, "w");
    if(fn == NULL)
    {
        printf("\nError in Opening the second file, %s", newt);
        getch();
        return 0;
    }
    char c;
    char cf[200];
    int flag = 0;
    int i=0;
    while(1){
        if(feof(fs)){
            break;
        }
        c = fgetc(fs);
        if (c != '\n') {
            cf[i++] = c;
        } else {
            cf[i++] = c;
            i = 0;
            char *found_string = strstr(cf, "X-Attachment-Id: ");
            if (found_string == NULL) {
                continue;
            } else {
                flag = 1;
                break;
            }
        }
    }
    while(flag){
        if(feof(fs)){
            break;
        }
        c = fgetc(fs);
        if(c == '-'){
            break;
        }
        else{
            fputc(c,fn);
        }
    }
    fclose(fs);
    fclose(fn);
    printf("fetching file finished\n");
    return 0;
}