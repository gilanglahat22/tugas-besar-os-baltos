#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__
#include "std_type.h"
// Filesystem data structure

#define FS_MAP_SECTOR_NUMBER    0x100
#define FS_NODE_SECTOR_NUMBER   0x101
#define FS_SECTOR_SECTOR_NUMBER 0x103
#define FS_CWD_SECTOR_NUMBER 0x200
#define MESSAGE_SECTOR(SEGMENT) ((SEGMENT) >> 11) + 0x300

#define FS_NODE_P_IDX_ROOT   0xFF
#define FS_NODE_S_IDX_FOLDER 0xFF

#define MAX_ARGC 10
#define MAX_ARGS_LENGTH 50
#define MAX_PROGRAMS 5

// message
// asumsi multiprogram maksimal 5 program
// maksimal argument 3
struct message{
    char argv[MAX_ARGC][MAX_ARGS_LENGTH];
    int next_program_segment; // Dapat digunakan untuk bagian 3.5
    byte other[10];
};

// cwd path
struct cwd_path{
    byte index;
    char path[511];
};

// Untuk filesystem map
struct map_filesystem {
    bool is_filled[512];
};


// Untuk filesystem nodes
struct node_entry {
    byte parent_node_index;
    byte sector_entry_index;
    char name[14];
};

struct node_filesystem {
    struct node_entry nodes[64];
};


// Untuk filesystem sector
struct sector_entry {
    byte sector_numbers[16];
};

struct sector_filesystem {
    struct sector_entry sector_list[32];
};


// Struktur data untuk read / write
struct file_metadata {
    byte *buffer;
    char *node_name;
    byte parent_index;
    unsigned int filesize;
};



// Untuk error code write & read
enum fs_retcode {
    FS_UNKNOWN_ERROR    = -1,
    FS_SUCCESS          = 0,
    FS_R_NODE_NOT_FOUND = 1,
    FS_R_TYPE_IS_FOLDER = 2,

    FS_W_FILE_ALREADY_EXIST   = 3,
    FS_W_NOT_ENOUGH_STORAGE   = 4,
    FS_W_MAXIMUM_NODE_ENTRY   = 5,
    FS_W_MAXIMUM_SECTOR_ENTRY = 6,
    FS_W_INVALID_FOLDER       = 7
};

#endif