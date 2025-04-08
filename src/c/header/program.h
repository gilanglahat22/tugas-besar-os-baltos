#ifndef __PROGRAM_H__
#define __PROGRAM_H__
#include "filesystem.h"
#include "utils.h"

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);


void exec(struct file_metadata *metadata, int segment);

void exit();


#endif