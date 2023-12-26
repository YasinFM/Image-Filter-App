#include <stdio.h>
#include <string.h>

int uid(){
    int ui;
    FILE *file = fopen("C:\\project\\fetch\\uid.txt","r");
    fscanf(file,"%d",ui);
    fclose(file);
    file = fopen("C:\\project\\fetch\\uid.txt","w");
    ui++;
    fputc(ui,file);
    fclose(file);
    return ui;
}