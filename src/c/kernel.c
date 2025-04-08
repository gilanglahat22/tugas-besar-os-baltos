#include "header/kernel.h"


int main() {
  struct file_metadata meta;
  struct message message;
  struct cwd_path path;
  fillKernelMap();
  makeInterrupt21();
  clearScreen();
    
  meta.node_name    = "shell";
  meta.parent_index = 0x00;      

  path.index = 0xFF;

  message.next_program_segment = 0x2000;
  path.path[0] = 0x0;


  printString("BaltOS Shell\r\n");
  printString("Copyright (C) BaltOS Corporation. All rights reserved.\r\n");
  printString("\r\n");

  writeSector(&message, MESSAGE_SECTOR(0x2000));
  writeSector(&path, FS_CWD_SECTOR_NUMBER);
  executeProgram(&meta, 0x2000);
}


void printString(char *string){
    int i=0;
    for(; string[i]!='\0'; i++){
        char ah = string[i];
        char al = 0xE;
        int ax = al * 256 + ah;
        interrupt(0x10, ax, 0x0, 0x0, 0x0);
    }
}

void readString(char *string){
    int i = 0;
    char curChar;
    while(true){
        curChar = interrupt(0x16, 0x0, 0x0, 0x0, 0x0);
        if(curChar == '\r'){
            string[i] = '\0';
            string[i+1] = '\r';
            string[i+2] = '\n';
            interrupt(0x10, 0xE*256 + string[i+1], 0x0, 0x0, 0x0);
            interrupt(0x10, 0xE*256 + string[i+2], 0x0, 0x0, 0x0);
            return;
        }
        // backspace
        else if(curChar == 0x08){
            if(i>0){
                string[i--] = '\0';
                interrupt(0x10, 0xE*256 + 0x08, 0x0, 0x0, 0x0);
                interrupt(0x10, 0xE*256 + 0x00, 0x0, 0x0, 0x0);
                interrupt(0x10, 0xE*256 + 0x08, 0x0, 0x0, 0x0);
            }
        }
        else{
            string[i++] = curChar;
            interrupt(0x10, 0xE*256 + curChar, 0x0, 0x0, 0x0);
        }
    }
}

void handleInterrupt21(int AX, int BX, int CX, int DX) {
    switch (AX) {
        case 0x0:
            printString(BX);
            break;
        case 0x1:
            readString(BX);
            break;
        case 0x2:
            readSector(BX, CX);
            break;
        case 0x3:
            writeSector(BX, CX);
            break;
        case 0x4:
            read(BX, CX);
            break;
        case 0x5:
            write(BX, CX);
            break;
        case 0x6:
            executeProgram(BX, CX);
            break;
        default:
            printString("Invalid Interrupt");
    }
}


void clearScreen(){
    interrupt(0x10,0x03,0,0,0);
    interrupt(0x10,6 << 8,0xF << 8,0,(24<<8) + 79);
    interrupt(0x10,2 << 8,0,0,0);
}

void readSector(byte *buffer, int sector_number) {
    int sector_read_count = 0x01;
    int cylinder, sector;
    int head, drive;

    cylinder = div(sector_number, 36) << 8; // CH
    sector   = mod(sector_number, 18) + 1;  // CL

    head  = mod(div(sector_number, 18), 2) << 8; // DH
    drive = 0x00;                                // DL

    interrupt(
        0x13,                       // Interrupt number
        0x0200 | sector_read_count, // AX
        buffer,                     // BX
        cylinder | sector,          // CX
        head | drive                // DX
    );
}

void writeSector(byte *buffer, int sector_number) {
    int sector_read_count = 0x01;
    int cylinder, sector;
    int head, drive;

    cylinder = div(sector_number, 36) << 8; // CH
    sector   = mod(sector_number, 18) + 1;  // CL

    head  = mod(div(sector_number, 18), 2) << 8; // DH
    drive = 0x00;                                // DL

    interrupt(
        0x13,                       // Interrupt number
        0x0300 | sector_read_count, // AX
        buffer,                     // BX
        cylinder | sector,          // CX
        head | drive                // DX
    );
}


void fillKernelMap(){
    struct map_filesystem map_fs_buffer;
    int i;


    readSector(map_fs_buffer.is_filled, FS_MAP_SECTOR_NUMBER);


    for(i=0; i<16; i++){
      map_fs_buffer.is_filled[i] = true;
    }

    for(i=256; i<512; i++){
      map_fs_buffer.is_filled[i] = true;
    }

    writeSector(map_fs_buffer.is_filled, FS_MAP_SECTOR_NUMBER); 
}

void read(struct file_metadata *metadata, enum fs_retcode *return_code) {
    struct node_filesystem   node_fs_buffer;
    struct sector_filesystem sector_fs_buffer;
    int i, idx; bool found = false;
    byte buf[8192];
    metadata->buffer = buf;


    readSector(sector_fs_buffer.sector_list, FS_SECTOR_SECTOR_NUMBER);
    readSector(&(node_fs_buffer.nodes[0]),  FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);


    for(i=0; i<64&&!found; i++){
      if(strcmp(metadata->node_name, node_fs_buffer.nodes[i].name)&&metadata->parent_index==node_fs_buffer.nodes[i].parent_node_index){
        if(node_fs_buffer.nodes[i].sector_entry_index==FS_NODE_S_IDX_FOLDER){
            *return_code = FS_R_TYPE_IS_FOLDER;
            return;
        }
        idx = node_fs_buffer.nodes[i].sector_entry_index;
        found = true;
      }
    }
    if(!found){
        *return_code = FS_R_NODE_NOT_FOUND;
        return;
    }

    metadata->filesize = 0;
    for(i=0; i<16; i++){
        if(sector_fs_buffer.sector_list[idx].sector_numbers[i]==0x0){
            break;
        }
        readSector(metadata->buffer + i* SECTOR_SIZE, sector_fs_buffer.sector_list[idx].sector_numbers[i]);
        metadata->filesize += SECTOR_SIZE;
    }
    *return_code = FS_SUCCESS;
    return;
}

void write(struct file_metadata *metadata, enum fs_retcode *return_code) {
	struct node_filesystem   node_fs_buffer;
	struct sector_filesystem sector_fs_buffer;
	struct map_filesystem    map_fs_buffer;

	// Tambahkan tipe data yang dibutuhkan
	int node_itr, sector_itr, map_itr, i = 0, j = 0, node_idx, sector_idx;
	int empty = 0;
	bool found = false;

	// Masukkan filesystem dari storage ke memori
	readSector(map_fs_buffer.is_filled, FS_MAP_SECTOR_NUMBER);
	readSector(sector_fs_buffer.sector_list, FS_SECTOR_SECTOR_NUMBER);
	readSector(&(node_fs_buffer.nodes[0]),  FS_NODE_SECTOR_NUMBER);
	readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

	for(node_itr = 0; node_itr < 64; node_itr++){
		if(strcmp(node_fs_buffer.nodes[node_itr].name, metadata->node_name) && metadata->parent_index == node_fs_buffer.nodes[node_itr].parent_node_index){
			*return_code=FS_W_FILE_ALREADY_EXIST;
      		return;
		}
	}

	for(node_itr = 0; node_itr < 64 && !found; node_itr++){
		if(strlen(node_fs_buffer.nodes[node_itr].name) == 0){
			node_idx = node_itr;
			found = true;
		}
	}
	if(!found){
		*return_code=FS_W_MAXIMUM_NODE_ENTRY;
        return;
	}

    if(metadata-> parent_index != FS_NODE_P_IDX_ROOT){
        if (node_fs_buffer.nodes[metadata->parent_index].sector_entry_index != FS_NODE_S_IDX_FOLDER){
            *return_code = FS_W_INVALID_FOLDER;
            return;
        }
    }

    for(map_itr=16; map_itr<256; map_itr++){
        if(!map_fs_buffer.is_filled[map_itr]){
            empty++;
        }
    }

    if(metadata->filesize>8192 || empty*SECTOR_SIZE<metadata->filesize){
        *return_code=FS_W_NOT_ENOUGH_STORAGE;
        return;
    }

    found = false;
	for(sector_itr = 0; sector_itr < 32 && !found; sector_itr++){
		if(sector_fs_buffer.sector_list[sector_itr].sector_numbers[0] == 0 && metadata->filesize > 0){
			sector_idx = sector_itr;
			found = true;
		}
	}
	if(!found && metadata->filesize > 0){
		*return_code = FS_W_MAXIMUM_SECTOR_ENTRY;
        return;
	}

    node_fs_buffer.nodes[node_idx].parent_node_index = metadata->parent_index;
    strncpy(node_fs_buffer.nodes[node_idx].name, metadata->node_name, 14);

    if (metadata->filesize == 0 ) {
        node_fs_buffer.nodes[node_idx].sector_entry_index = FS_NODE_S_IDX_FOLDER;
    }
    else{
        node_fs_buffer.nodes[node_idx].sector_entry_index = sector_idx;
        for(i=0; i<256 && j*SECTOR_SIZE < metadata->filesize; i++){
			if(!map_fs_buffer.is_filled[i]){
				map_fs_buffer.is_filled[i] = true;
				sector_fs_buffer.sector_list[sector_idx].sector_numbers[j] = i;
				j++;
				writeSector(&(metadata->buffer[j * SECTOR_SIZE]), i);
			}
		}
		while(j < 16){
			sector_fs_buffer.sector_list[sector_idx].sector_numbers[j] = 0x0;
			j++;
		}
    }
    
    writeSector(map_fs_buffer.is_filled, FS_MAP_SECTOR_NUMBER);
    writeSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
    writeSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER+1);
    writeSector(sector_fs_buffer.sector_list, FS_SECTOR_SECTOR_NUMBER);

    *return_code = FS_SUCCESS;
    return;
}

void executeProgram(struct file_metadata *metadata, int segment) {
  enum fs_retcode fs_ret;
  byte buf[8192];
  
  metadata->buffer = buf;
  read(metadata, &fs_ret);
  if (fs_ret == FS_SUCCESS) {
    int i = 0;
    for (i = 0; i < 8192; i++) {
      if (i < metadata->filesize)
        putInMemory(segment, i, metadata->buffer[i]);
      else
        putInMemory(segment, i, 0x00);
    }
    launchProgram(segment);
  }
  else
    printString("exec: file not found\r\n");
}

