#include <stdio.h>
#include <string.h>

int uim(){
    char c;
    int uid = 0;
    FILE *file = fopen("C:\\project\\fetch\\uid.txt","r");
    while (1){
        if (feof(file)){
            break;
        }
        c = fgetc(file);
        if ( c>='0' && c<='9' ){
            uid = (uid*10) + (c-'0');
        }
    }
    rewind(file);
    fclose(file);
    file = fopen("C:\\project\\fetch\\uid.txt","w");
    int i = uid - 1;
    fprintf(file,"%d",i);
    fclose(file);
    return 0;
}