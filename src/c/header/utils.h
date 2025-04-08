#ifndef __UTILS_H__
#define __UTILS_H__
#include "filesystem.h"
#include "string.h"
#include "fileio.h"

extern int getCurrentSegment();
extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

void getMessage(struct message *message, int segment);

void setMessage(struct message *message, int segment);

void getCWD(struct cwd_path *path);

void setCWD(struct cwd_path *path);

byte fileIndex(struct file_metadata *metadata, enum fs_retcode *status, char* path, byte parent);

byte dirIndex(struct file_metadata metadata, enum fs_retcode *status, bool folder);

void deleteContent(byte fileidx);

#endif