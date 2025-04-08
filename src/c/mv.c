#include "header/std_lib.h"

int main(){
    struct node_filesystem nd;
    struct message msg;
    struct file_metadata metadata, tmp;
    struct cwd_path cur_path;
    enum fs_retcode status;
    char pathsrc[511], name[14], pathdest[511], dirdest[511];
    int i, j, argc = 0;
    byte filesrc, dir, targetfile, c, cur;
    bool is_file;

    cur_path.path[0] = 0x0;
    getMessage(&msg, getCurrentSegment());
    getCWD(&cur_path);

    readSector((&nd.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector((&nd.nodes[32]), FS_NODE_SECTOR_NUMBER+1);
    for(i=1; i<MAX_ARGC; i++){
        if(msg.argv[i][0] != 0x0){
            argc++;
        }
    }

    if(argc == 0){
        puts("mv: missing operand\r\n");
        exit();
    }else if(argc == 1){
        puts("mv: missing target\r\n");
        exit();
    }else if(argc > 2){
        puts("mv : too many arguments\r\n");
        exit();
    }else{
        if(msg.argv[1][0] == '/'){
            strcpy(pathsrc, msg.argv[1]);
        }else{
            j = 0;
            while(cur_path.path[j] != 0x0) j++;
            cur_path.path[j] = '/';
            cur_path.path[j+1] = 0x0;
            strcat(pathsrc, cur_path.path, msg.argv[1]);
        }
        absPath(pathsrc, pathsrc);
        filesrc = fileIndex(&metadata, &status, pathsrc, FS_NODE_P_IDX_ROOT);
        if(status == FS_R_NODE_NOT_FOUND){
            puts("mv: cannot move file or directory \'");
            puts(msg.argv[1]);
            puts("\': No such file or directory\r\n");
        }else{
            is_file = status == FS_SUCCESS;
            if(msg.argv[2][0] == '/'){
                strcpy(pathdest, msg.argv[2]);
            }else{
                j = 0;
                while(cur_path.path[j] != 0x0) j++;
                if(j>0 && cur_path.path[j-1] != '/'){
                    cur_path.path[j] = '/';
                    cur_path.path[j+1] = 0x0;
                }
                strcat(pathdest, cur_path.path, msg.argv[2]);
            }
            
            absPath(pathdest, pathdest);
            parsePath(name, dirdest, pathdest);
            dir = fileIndex(&tmp, &status, dirdest, FS_NODE_P_IDX_ROOT);
            if(status == FS_R_TYPE_IS_FOLDER){
                targetfile = fileIndex(&tmp, &status, pathdest, FS_NODE_P_IDX_ROOT);
                if((status == FS_SUCCESS && is_file) || (status == FS_R_TYPE_IS_FOLDER && !is_file)){
                    puts("mv: cannot move file or directory to \'");
                    puts(msg.argv[2]);
                    puts("\': File or directory exists\r\n");
                }else if(status == FS_R_TYPE_IS_FOLDER && is_file){
                    nd.nodes[filesrc].parent_node_index = targetfile;
                    writeSector(&(nd.nodes[0]), FS_NODE_SECTOR_NUMBER);
                    writeSector(&(nd.nodes[32]), FS_NODE_SECTOR_NUMBER+1);
                }else if(strlen(name) > 14){
                    puts("mv: cannot move file or directory to \'");
                    puts(msg.argv[2]);
                    puts("\': Name length exceeds 14.\r\n");
                }else{
                    strcpy(nd.nodes[filesrc].name, name);
                    nd.nodes[filesrc].parent_node_index = dir;
                    writeSector(&(nd.nodes[0]), FS_NODE_SECTOR_NUMBER);
                    writeSector(&(nd.nodes[32]), FS_NODE_SECTOR_NUMBER+1);
                }
            }else if(status == FS_SUCCESS){
                puts("mv: cannot move file or directory to \'");
                puts(dirdest);
                puts("\': Not a directory\r\n");
            }else if(status == FS_R_NODE_NOT_FOUND){
                puts("mv: cannot move file or directory \'");
                puts(dirdest);
                puts("\': Not such file or directory\r\n");
            }
        }
        exit();
    }
}