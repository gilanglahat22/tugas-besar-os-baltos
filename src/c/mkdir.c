#include "header/std_lib.h"

int main(){
    struct message msg;
    struct file_metadata metadata;
    struct cwd_path cur_path;
    enum fs_retcode status;
    char path[511], name[14], dirPath[511];
    int i, j, argc = 0;
    byte dir;
    cur_path.path[0] = 0x0;

    getMessage(&msg, getCurrentSegment());
    getCWD(&cur_path);

    for(i=1; i<MAX_ARGC; i++){
        if(msg.argv[i][0] != 0x0){
            argc++;
        }
    }


    if(argc == 0){
        puts("mkdir: missing operand\r\n");
        exit();
    }
    else{
        for(i=1; i<=argc; i++){
            path[0] = 0x0;
            dirPath[0] = 0x0;
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
            parsePath(name, dirPath, path);
            dir = fileIndex(&metadata, &status, dirPath, FS_NODE_P_IDX_ROOT);
            if(status == FS_R_TYPE_IS_FOLDER){
                metadata.node_name = name;
                strcpy(metadata.node_name, name);
                metadata.parent_index = dir;
                metadata.filesize = 0;
                write(&metadata, &status);
                if(status == FS_W_FILE_ALREADY_EXIST){
                    puts("mkdir: cannot create directory \'");
                    puts(msg.argv[i]);
                    puts("\': File exists\r\n");
                }
                else if(status == FS_W_NOT_ENOUGH_STORAGE || status == FS_W_MAXIMUM_SECTOR_ENTRY || status == FS_W_MAXIMUM_NODE_ENTRY){
                    puts("mkdir: cannot create directory \'");
                    puts(msg.argv[i]);
                    puts("\': Not enough space\r\n");
                }

            }
            else if(status == FS_SUCCESS){
                puts("mkdir: cannot create directory \'");
                puts(msg.argv[i]);
                puts("\': Not a directory\r\n");
            }
            else if(status == FS_R_NODE_NOT_FOUND){
                puts("mkdir: cannot create directory \'");
                puts(msg.argv[i]);
                puts("\': No such file or directory\r\n");
            }
        }
        exit();
    }
}