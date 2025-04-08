#ifndef __STRING_H__
#define __STRING_H__

#include "std_type.h"

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

// Operasi standar bahasa C
// Implementasikan
void memcpy(byte *dest, byte *src, int n);
// Mengcopy n bytes yang ditunjuk src ke dest

int strlen(char *string);
// Mengembalikan panjang suatu null terminated string

bool strcmp(char *s1, char *s2);
// Mengembalikan true jika string sama

bool strncmp(char *s1, char *s2, int n);

void strcpy(char *dst, char *src);
// Melakukan penyalinan null terminated string

void clear(char *ptr, int n);
// Mengosongkan char array yang memiliki panjang n

void strncpy(char *dst, char *src, int n);

void strcat(char *target, char *dest, char *src);

void absPath(char *target, char *path);

void parsePath(char *targetName, char *dirPath, char *srcPath);

#endif