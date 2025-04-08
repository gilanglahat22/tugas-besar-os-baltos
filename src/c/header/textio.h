#ifndef __TEXTIO_H__
#define __TEXTIO_H__

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

void puts(char *str);

char *gets(char *str);


#endif