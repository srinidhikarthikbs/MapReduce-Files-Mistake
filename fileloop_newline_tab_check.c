#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>
int main(){
    DIR *dir = opendir("/DATA/full_data/");
    if(dir == NULL){
        printf("error opening the directory");
        exit(1);
    }
    long count = 0;
    struct dirent *dir_ptr=NULL;
    dir_ptr = readdir(dir);//to skip . directory
    dir_ptr = readdir(dir);//to skip .. directory
    while((dir_ptr = readdir(dir))!=NULL){
        count++;
        char *file_name;
        file_name = (char*) malloc(50);
        strcpy(file_name,"");
        strcat(file_name, "/DATA/full_data/");
        strcat(file_name, dir_ptr->d_name);
        
        FILE *fp = NULL;
        fp=fopen(file_name,"r");
        if(fp == NULL){
            perror("error opening the file");
            exit(1);
        }
        fseek(fp, 0L, SEEK_END);
        long file_size=0;
        file_size = ftell(fp);
        fseek(fp, 0L, SEEK_SET);
        char *str;
        str=(char*)calloc(file_size+2, sizeof(char));
        fread(str, sizeof(char), file_size, fp);
        long i=0, len = strlen(str);
        long count_newlines=0, count_tabs=0, count_otherescapechars=0;
        for(i=0;i<len;i++){
            if(str[i]==10) count_newlines++;
            if(str[i]==0x09) count_tabs++;
            if(str[i]==0x07 ||str[i]==0x08 ||str[i]==0x0c ||str[i]==0x0d ||str[i]==0x0b ||str[i]==0x5c ||str[i]==0x27 ||str[i]==0x22 ||str[i]==0x3f)
                count_otherescapechars++;
        }
        //print only if there is an anamoly
        if(count_newlines!=0 || count_tabs!=1 || count_otherescapechars!=0)
            printf("%s - newline=%ld tab=%ld other=%ld\n", file_name, count_newlines, count_tabs, count_otherescapechars);
        fclose(fp);
        strcpy(str,"");
        free(str);
        free(file_name);
    }
    printf("\ncount = %ld\n", count);
    return 0;
}