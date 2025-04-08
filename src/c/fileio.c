#include "header/fileio.h"

void read(struct file_metadata *metadata, enum fs_retcode *status){
    interrupt(0x21, 0x4, metadata, status, 0x0);
}

void write(struct file_metadata *metadata, enum fs_retcode *status){
    interrupt(0x21, 0x5, metadata, status, 0x0);
}

void readSector(byte *buffer, int segment_number){
    interrupt(0x21, 0x02, buffer, segment_number, 0x0);
}

void writeSector(byte *buffer, int segment_number){
    interrupt(0x21, 0x03, buffer, segment_number, 0x0);
}

void ls(byte idx){
    struct node_filesystem nd;
    int i;
    interrupt(0x21, 0x02, &(nd.nodes[0]), FS_NODE_SECTOR_NUMBER, 0);
    interrupt(0x21, 0x02, &(nd.nodes[32]), FS_NODE_SECTOR_NUMBER+1, 0);

    for(i=0; i<64; i++){
        if(nd.nodes[i].parent_node_index == idx){
            interrupt(0x21, 0x00, nd.nodes[i].name, 0, 0);
            interrupt(0x21, 0x00, " ", 0, 0);
        }
    }
    interrupt(0x21, 0x00, "\r\n", 0, 0);
}