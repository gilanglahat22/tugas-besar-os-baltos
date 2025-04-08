#include "header/utils.h"


void getMessage(struct message *message, int segment){
    int i;
    for(i=0; i<MAX_ARGC; i++){
        clear(message->argv[i], MAX_ARGS_LENGTH);
    }
    interrupt(0x21, 0x02, message, MESSAGE_SECTOR(segment), 0x0);    
}

void setMessage(struct message *message, int segment){
    interrupt(0x21, 0x03, message, MESSAGE_SECTOR(segment), 0x0);
}

void getCWD(struct cwd_path *path){
    interrupt(0x21, 0x02, path, FS_CWD_SECTOR_NUMBER, 0x0);
}

void setCWD(struct cwd_path *path){
    interrupt(0x21, 0x03, path, FS_CWD_SECTOR_NUMBER, 0x0);
}

byte fileIndex(struct file_metadata *metadata, enum fs_retcode *status, char* path, byte parent){
    struct node_filesystem nd;
    int i = 0, j = 0, k;
    bool found;
    byte cur = parent;
    char tmp_name[14], cur_name[14], tmp[14];
    interrupt(0x21, 0x02, &(nd.nodes[0]), FS_NODE_SECTOR_NUMBER, 0);
    interrupt(0x21, 0x02, &(nd.nodes[32]), FS_NODE_SECTOR_NUMBER+1, 0);

    if(path[0] == 0x0){
        cur = FS_NODE_P_IDX_ROOT;
        *status = FS_R_TYPE_IS_FOLDER;
        return cur;
    }
    while(path[i]!=0x0){
        if(path[i] == '/') i++;
        j = 0;
        clear(tmp_name, 14);
        while(path[i]!=0x0 && path[i]!='/'){
            if(j >= 14){
                *status = FS_UNKNOWN_ERROR;
                return parent;
            }
            tmp_name[j] = path[i];
            i++; j++;
        }
        tmp_name[j] = 0x0;
        found = false;
        for(k = 0; k<64&&!found; k++){
            if(strcmp(tmp_name, nd.nodes[k].name)&&cur==nd.nodes[k].parent_node_index){
                clear(cur_name, 14);
                cur = k;
                strcpy(cur_name, tmp_name);
                found = true;
            }
        }
        if(!found){
            *status = FS_R_NODE_NOT_FOUND;
            return parent;
        }
    }
    metadata->node_name = tmp;
    strcpy(metadata->node_name, cur_name);
    metadata->parent_index = nd.nodes[cur].parent_node_index;
    // puts(metadata->node_name);
    // puts("\r\n");
    // putsInt(metadata->parent_index);
    // puts("\r\n");
    read(metadata, status);
    if(*status == FS_R_NODE_NOT_FOUND){
        return parent;
    }
    return cur;
}

byte dirIndex(struct file_metadata metadata, enum fs_retcode *status, bool folder){
    struct node_filesystem nd;
    int idx;
    bool found = false;
    interrupt(0x21, 0x02, &(nd.nodes[0]), FS_NODE_SECTOR_NUMBER, 0);
    interrupt(0x21, 0x02, &(nd.nodes[32]), FS_NODE_SECTOR_NUMBER+1, 0);

    for(idx = 0; idx < 64 && !found; idx++){
        if(metadata.parent_index == nd.nodes[idx].parent_node_index){
            if(folder && nd.nodes[idx].sector_entry_index == 0xFF && strncmp(metadata.node_name, nd.nodes[idx].name, 14)){
                found = true;
            }
            else if(!folder && nd.nodes[idx].sector_entry_index == 0xFF && strncmp(metadata.node_name, nd.nodes[idx].name, 14)){
                found = true;
            }
        }
    }
    if(!found){
        *status = FS_R_NODE_NOT_FOUND;
        return 0xFF;
    }
    else{
        *status = FS_SUCCESS;
        return idx;
    }
}

void deleteContent(byte fileidx){
    struct node_filesystem nd;
    struct sector_filesystem sy;
    struct map_filesystem mp;
    byte buffer[512];
    int i, idx, sect;

    clear(buffer, 512);

    interrupt(0x21, 0x02, &(nd.nodes[0]), FS_NODE_SECTOR_NUMBER, 0);
    interrupt(0x21, 0x02, &(nd.nodes[32]), FS_NODE_SECTOR_NUMBER+1, 0);
    interrupt(0x21, 0x02, sy.sector_list, FS_SECTOR_SECTOR_NUMBER, 0);
    interrupt(0x21, 0x02, mp.is_filled, FS_MAP_SECTOR_NUMBER, 0);

    i = nd.nodes[fileidx].sector_entry_index;

    for(idx = 0; idx < 16; idx++){
        sect = sy.sector_list[i].sector_numbers[idx];
        if(sect == 0x0){
            break;
        }
        mp.is_filled[sect] = false;
        sy.sector_list[i].sector_numbers[idx] = 0x0;
        interrupt(0x21, 0x03, &buffer, idx, 0);
    }

    clear(nd.nodes[fileidx].name, 14);
    nd.nodes[fileidx].parent_node_index = 0x0;
    nd.nodes[fileidx].sector_entry_index = 0x0;

    interrupt(0x21, 0x03, sy.sector_list, FS_MAP_SECTOR_NUMBER, 0);
    interrupt(0x21, 0x03, mp.is_filled, FS_MAP_SECTOR_NUMBER, 0);
    interrupt(0x21, 0x03, &(nd.nodes[0]), FS_NODE_SECTOR_NUMBER, 0);
    interrupt(0x21, 0x03, &(nd.nodes[32]), FS_NODE_SECTOR_NUMBER+1, 0);
}