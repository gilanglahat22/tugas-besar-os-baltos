// Kernel header


#include "filesystem.h"
#include "math.h"
#include "string.h"

#define MAX_ARGC 20
#define MAX_ARGV 50
#define MAX_PATH 14
#define MAX_PATHS 64

#define SECTOR_SIZE 512

// Fungsi bawaan
extern void putInMemory(int segment, int address, char character);
extern int interrupt(int int_number, int AX, int BX, int CX, int DX);
extern void launchProgram(int segment);
void makeInterrupt21();
void handleInterrupt21(int AX, int BX, int CX, int DX);
void fillKernelMap();
void executeProgram(struct file_metadata *metadata, int segment);

void printString(char *string);
void readString(char *string);
void clearScreen();

void writeSector(byte *buffer, int sector_number);
void readSector(byte *buffer, int sector_number);

void write(struct file_metadata *metadata, enum fs_retcode *return_code);
void read(struct file_metadata *metadata, enum fs_retcode *return_code);


void executeProgram(struct file_metadata *metadata, int segment);