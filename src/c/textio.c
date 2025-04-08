#include "header/textio.h"

void puts(char *str){
    interrupt(0x21, 0x00, str, 0, 0);
}

char *gets(char *str){
    interrupt(0x21, 0x01, str, 0, 0);
    return str;
}
