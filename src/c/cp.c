#include "header/std_lib.h"

int main(){
    struct message msg;
    struct file_metadata metadata[2], tmpmetadata;
    struct node_filesystem nd;
    enum fs_retcode status[2], tmpstatus, writeStatus;
    struct cwd_path cur_path[2];
    char temp[2][511], name[2][14], dirPath[2][511];
    int argc = 0, i = 0, j=0;
    byte dir[2], tmpdir;
    cur_path[0].path[0] = 0x0;
    getMessage(&msg, getCurrentSegment());
    getCWD(&(cur_path[0]));
    getCWD(&(cur_path[1]));
    for(i = 1; i<MAX_ARGC; i++){
        if(msg.argv[i][0] != 0x0){
            argc++;
        }
    }
    if(argc <= 1){
        puts("cp: missing operand\r\n");
        exit();
    }
    else if(argc > 2){
        puts("cp: too many arguments\r\n");
        exit();
    }
    else{
        writeStatus = FS_SUCCESS;
        for(j=0; j<2; j++){
            if(msg.argv[j+1][0] == '/' || cur_path[j].index == FS_NODE_P_IDX_ROOT){
                strcpy(cur_path[j].path, msg.argv[j+1]);
            }
            else{
                i = 0;
                while(cur_path[j].path[i] != 0x0) i++;
                cur_path[j].path[i] = '/';
                cur_path[j].path[i+1] = 0x0;
                strcat(temp[j], cur_path[j].path, msg.argv[j+1]);
                strcpy(cur_path[j].path, temp[j]);

            }
            absPath(cur_path[j].path, cur_path[j].path);
            parsePath(name[j], dirPath[j], cur_path[j].path);
            dir[j] = fileIndex(&(metadata[j]), &(status[j]), cur_path[j].path, FS_NODE_P_IDX_ROOT);
        }
        if(status[0] == FS_R_NODE_NOT_FOUND){
            puts("cp: cannot stat '");
            puts(msg.argv[1]);
            puts("': No such file or directory\r\n");
        }
        else if(status[0] == FS_R_TYPE_IS_FOLDER){
            puts("cp: Source file '");
            puts(msg.argv[1]);
            puts("' Is a directory\r\n");
        } else if(status[0] == FS_SUCCESS){
            if(status[1] == FS_SUCCESS){
                deleteContent(dir[1]);
                metadata[0].node_name = name[1];
                strcpy(metadata[0].node_name, name[1]);
                metadata[0].parent_index = metadata[1].parent_index;
                write(&(metadata[0]), &writeStatus);
            }
            else if(status[1] == FS_R_NODE_NOT_FOUND){
                strcpy(metadata[1].node_name, metadata[0].node_name);
                strcpy(metadata[1].buffer, metadata[0].buffer);
                metadata[1].filesize = metadata[0].filesize;
                metadata[1].parent_index = metadata[0].parent_index;
                write(&(metadata[1]), &writeStatus);
            }
            else if(status[1] == FS_R_TYPE_IS_FOLDER){
                i = 0;
                while(cur_path[1].path[i] != 0x0) i++;
                cur_path[1].path[i] = '/';
                cur_path[1].path[i+1] = 0x0;
                strcat(cur_path[1].path, cur_path[1].path, name[0]);
                tmpdir = fileIndex(&tmpmetadata, &tmpstatus, cur_path[1].path, FS_NODE_P_IDX_ROOT);
                if(tmpstatus == FS_SUCCESS){
                    deleteContent(tmpdir);
                    metadata[0].node_name = name[1];
                    strcpy(metadata[0].node_name, name[1]);
                    metadata[0].parent_index = dir[1];
                    write(&(metadata[0]), &writeStatus);
                }
                else if(tmpstatus == FS_R_TYPE_IS_FOLDER){
                    puts("cp: cannot overwrite directory with non-directory\r\n");
                }
                else if(tmpstatus == FS_R_NODE_NOT_FOUND){
                    metadata[0].node_name = name[0];
                    strcpy(metadata[0].node_name, name[0]);
                    metadata[0].parent_index = dir[1];
                    write(&(metadata[0]), &writeStatus);
                }
            }
        }
        if(writeStatus == FS_W_FILE_ALREADY_EXIST){
            puts("cp: cannot create file or directory \'");
            puts(msg.argv[i]);
            puts("\': File exists\r\n");
        }
        else if(status == FS_W_NOT_ENOUGH_STORAGE || status == FS_W_MAXIMUM_SECTOR_ENTRY || status == FS_W_MAXIMUM_NODE_ENTRY){
            puts("cp: cannot create file or directory \'");
            puts(msg.argv[i]);
            puts("\': Not enough space\r\n");
        }
        exit();
    }
}