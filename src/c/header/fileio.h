#ifndef __FILEIO_H__
#define __FILEIO_H__

#include "filesystem.h"

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

void read(struct file_metadata *metadata, enum fs_retcode *status);

void write(struct file_metadata *metadata, enum fs_retcode *status);

void readSector(byte *buffer, int segment_number);

void writeSector(byte *buffer, int segment_number);

void ls(byte idx);

#endif