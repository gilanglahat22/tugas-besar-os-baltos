#include "header/std_lib.h"

int main(){
    struct message msg;
    struct file_metadata metadata;
    struct node_filesystem nd;
    enum fs_retcode status;
    struct cwd_path cur_path;
    int argc = 0, i = 0, j;
    byte dir;
    char path[511];
    cur_path.path[0] = 0x0;

    getMessage(&msg, getCurrentSegment());
    getCWD(&cur_path);
    for(i = 1; i<MAX_ARGC; i++){
        if(msg.argv[i][0] != 0x0){
            argc++;
        }
    }
    if(argc == 0){
        puts("cat: missing operand\r\n");
        exit();
    }
    else{
        for (i = 1; i <= argc; i++){
            path[0] = 0x0;
            metadata.node_name[0] = 0x0;
            if(msg.argv[i][0] == '/'){
                strcpy(path, msg.argv[i]);
            }
            else{
                j = 0;
                while(cur_path.path[j] != 0x0) j++;
                cur_path.path[j] = '/';
                cur_path.path[j+1] = 0x0;
                strcat(path, cur_path.path, msg.argv[i]);
            }

            absPath(path, path);
            dir = fileIndex(&metadata, &status, path, FS_NODE_P_IDX_ROOT);
            if(status == FS_R_NODE_NOT_FOUND){
                puts("cat: ");
                puts(msg.argv[i]);
                puts(": No such file or directory\r\n");
            }
            else{
                if(status == FS_R_TYPE_IS_FOLDER){
                    puts("cat: ");
                    puts(msg.argv[i]);
                    puts(": Is a directory\r\n");
                }
                else{
                    read(&(metadata), &status);
                    puts(metadata.buffer);
                    puts("\r\n");
                }
            }
        }
        exit();     
    }
}