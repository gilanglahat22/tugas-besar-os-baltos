#include "program.h"
#include "string.h"
#include "utils.h"

#define ERR_TO_MANY_ARGUMENT -1
#define ERR_MISSING_OPERAND -2
#define ERR_NOT_VALID_FORMAT -3

int parseCommand(char args[MAX_PROGRAMS][MAX_ARGC][MAX_ARGS_LENGTH], char input_raw[1024]);
bool isCommand(char* arg);
