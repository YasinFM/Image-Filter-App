#include<stdio.h>
#include<conio.h>

int main(){
    char fileName1[20]="first file", fileName2[20]="second file";
    FILE *fs, *fn;
    fs = fopen("C:\\project\\fetch\\tt.txt", "rb");
    if(fs == NULL)
    {
        printf("\nError in Opening the first file, %s", fileName1);
        getch();
        return 0;
    }
    fn = fopen("C:\\project\\fetch\\tn.txt", "w");
    if(fn == NULL)
    {
        printf("\nError in Opening the second file, %s", fileName2);
        getch();
        return 0;
    }
    char c;
    int count = 0,flag = 0;
    while (1){
        if(feof(fs)){
            break;
        }
        c = fgetc(fs);
        if(c == '\n'){
            count++;
            if(count == 2){
                flag = 1;
                break;
            }
        } else{
            count = 0;
        }
    }
    int i=0;
    while (flag){
        if(feof(fs)){
            break;
        }
        c = fgetc(fs);
        fputc(c,fn);
        i++;
        if(i==274){
            continue;
        }
    }
    fclose(fs);
    fclose(fn);
    printf("cut done!\n");
    return 0;
}