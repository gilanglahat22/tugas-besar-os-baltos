#include "header/std_lib.h"


int main(){
    struct message msg;
    struct file_metadata metadata;
    enum fs_retcode status;
    struct cwd_path cur_path;
    char temp[511];
    int argc = 0, i = 0;
    byte dir;
    cur_path.path[0] = 0x0;

    getMessage(&msg, getCurrentSegment());
    getCWD(&cur_path);
    for(i = 1; i<MAX_ARGC; i++){
        if(msg.argv[i][0] != 0x0){
            argc++;
        }
    }
    if(argc == 0){
        exit();
    }
    else if(argc > 1){
        puts("cd: too many arguments\r\n");
        exit();
    }
    else{
        if(msg.argv[1][0] == '/' || cur_path.index == FS_NODE_P_IDX_ROOT){
            strcpy(cur_path.path, msg.argv[1]);
        }
        else{
            i = 0;
            while(cur_path.path[i] != 0x0) i++;
            cur_path.path[i] = '/';
            cur_path.path[i+1] = 0x0;
            strcat(temp, cur_path.path, msg.argv[1]);
            strcpy(cur_path.path, temp);
        }

        absPath(cur_path.path, cur_path.path);
        dir = fileIndex(&metadata, &status, cur_path.path, FS_NODE_P_IDX_ROOT);

        if(status == FS_R_TYPE_IS_FOLDER){
            cur_path.index = dir;
            setCWD(&cur_path);
        }
        else if(status == FS_SUCCESS){
            puts("cd: ");
            puts(msg.argv[1]);
            puts(": Not a directory\r\n");
        }
        else if(status == FS_R_NODE_NOT_FOUND){
            puts("cd: ");
            puts(msg.argv[1]);
            puts(": No such file or directory\r\n");
        }
        else if(status == FS_UNKNOWN_ERROR){
            puts("cd: Invalid argument");
        }
        exit();
    }
    
}