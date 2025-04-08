#include "header/textio.h"
#include "header/program.h"

int main(){
    char buffer[512];
    puts("ketik sesuatu: ");
    gets(buffer);
    puts("hasil ketikan: ");
    puts(buffer);
    puts("\r\n"); 
    exit();
}