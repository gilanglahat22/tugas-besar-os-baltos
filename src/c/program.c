#include "header/program.h"


void exec(struct file_metadata *metadata, int segment){
    interrupt(0x21, 0x06, metadata, segment, 0x0);
}

void exit(){
    struct message cur_msg, new_msg, temp_msg;
    struct file_metadata metadata;
    int i = 0;
    getMessage(&cur_msg, getCurrentSegment());
    metadata.node_name[0] = 0x0;
    metadata.parent_index = 0x0;
    if(cur_msg.next_program_segment == 0x2000){
        strcpy(metadata.node_name, "shell");
    }
    else{
        getMessage(&new_msg, cur_msg.next_program_segment);
        strcpy(metadata.node_name, new_msg.argv[0]);
    }

    for(i=0; i<MAX_ARGC; i++){
        clear(temp_msg.argv[i], MAX_ARGS_LENGTH);
    }
    temp_msg.next_program_segment = 0x2000;
    setMessage(&temp_msg, getCurrentSegment());

    exec(&metadata, cur_msg.next_program_segment);
}