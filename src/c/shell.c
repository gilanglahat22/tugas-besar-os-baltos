#include "header/shell.h"

int main(){
    struct message message, send;
    struct file_metadata metadata;
    struct cwd_path path;
    enum fs_retcode status;
    char input_raw[1025], temp[511];
    char args[MAX_PROGRAMS][MAX_ARGC][MAX_ARGS_LENGTH];
    int programc, i, j, segment, result;
    byte current_dir, exec_dir;
    bool valid = true;
    
    while (true) {
        puts("OS@IF2230:/");
        path.path[0] = 0x0;
        clear(input_raw, 1025);
        for(i=0; i<MAX_ARGC; i++){
            clear(message.argv[i], MAX_ARGS_LENGTH);
        }
        getCWD(&path);
        getMessage(&message, 0x2000);
        current_dir = path.index;
        puts(path.path);

        puts("$");
        gets(input_raw);


        programc = parseCommand(args, input_raw);
        if(programc < 0){
            if(programc == ERR_NOT_VALID_FORMAT){
                puts("Your command is not valid\r\n");
            }
            else if(programc == ERR_TO_MANY_ARGUMENT){
                puts("Too many arguments\r\n");
            }
        }
        else{
            valid = true;
            for(i=0; i<programc; i++){
                if(!isCommand(args[i][0])){
                    if(programc == 1 && strncmp(args[i][0], "./", 2)){
                        continue;
                    }
                    puts(args[i][0]);
                    puts(": Command not found\r\n");
                    valid = false;
                }
            }
            if(!valid){
                continue;
            }
            else{
                if(programc == 1 && strncmp(args[0][0], "./", 2)){
                    if(current_dir != FS_NODE_P_IDX_ROOT){
                        i = 0;
                        while(path.path[i] != 0x0) i++;
                        path.path[i] = '/';
                        path.path[i+1] = 0x0;
                        strcat(temp, path.path, args[0][0]);
                    }
                    else{
                        strcpy(temp, args[0][0]);
                    }

                    absPath(temp, temp);
                    clear(metadata.node_name, 14);
                    exec_dir = fileIndex(&metadata, &status, temp, FS_NODE_P_IDX_ROOT);

                    if(status == FS_SUCCESS){
                        for(j=0; j<MAX_ARGC; j++){
                            clear(send.argv[j], MAX_ARGS_LENGTH);
                        }
                        send.next_program_segment = 0x2000;
                        setMessage(&send, 0x2000);
                        exec(&metadata, 0x2000);
                    }
                    else if(status == FS_UNKNOWN_ERROR){
                        puts("execute command is invalid\r\n");
                    }
                    else if(status == FS_R_NODE_NOT_FOUND){
                        puts(args[0][0]);
                        puts(": No such file or directory\r\n");
                    }
                    else if(status == FS_R_TYPE_IS_FOLDER){
                        puts(args[0][0]);
                        puts(": Is a directory\r\n");
                    }
                }
                else{
                    segment = 0x3000;
                    for(i=0; i<programc; i++, segment+=0x1000){
                        for(j=0; j<MAX_ARGC; j++){
                            clear(send.argv[j], MAX_ARGS_LENGTH);
                        }
                        for(j=0; j<MAX_ARGC; j++){
                            strcpy(send.argv[j], args[i][j]);
                        }
                        if(i == programc-1){
                            send.next_program_segment = 0x2000;
                        }
                        else{
                            send.next_program_segment = segment + 0x1000;
                        }
                        setMessage(&send, segment);
                    }
                    strcpy(metadata.node_name, args[0][0]);
                    metadata.parent_index = 0x0;
                    exec(&metadata, 0x3000);
                }
            }
        }

    }
}

int parseCommand(char args[MAX_PROGRAMS][MAX_ARGC][MAX_ARGS_LENGTH], char input_raw[1025]){
	int i = 0, j = 0, k = 0, l = 0, cnt = 0, notempty = 0;
	char tmp[MAX_PROGRAMS][205];
	for(i=0; i<MAX_PROGRAMS; i++){
		for(j=0; j<205; j++){
			tmp[i][j] = 0x0;
		}
        for(j=0; j<MAX_ARGC; j++){
            clear(args[i][j], MAX_ARGS_LENGTH);
        }
	}
	i = 0;
	j = 0;
	while(input_raw[i] != 0x0){
		if(strncmp(input_raw + i, " ; ", 3) == 1){
			if(cnt >= MAX_PROGRAMS-1){
				return ERR_TO_MANY_ARGUMENT;
			}
			tmp[cnt][j] = 0x0;
			cnt++;
			i += 3;
			j = 0;
		}
		else{
			while(strncmp(input_raw + i, " ; ", 3) == 0 && input_raw[i] != 0x0){
				tmp[cnt][j] = input_raw[i];
				i++; j++;
			}
			if(input_raw[i] == 0x0){
				tmp[cnt][j] = 0x0;
			}
		}
	}

	for(j=0; j<MAX_PROGRAMS; j++){
		i = 0, l=0;
		while(tmp[j][i] != 0x0){
			while(tmp[j][i] == ' ') i++;
			k = 0;
			while(tmp[j][i] != ' ' && tmp[j][i] != 0x0){
				args[j][l][k] = tmp[j][i];
				i++; k++;
			}
			args[j][l][k] = 0x0;
			if(k > 0){
				l++;
			}
		}
	}

    for(i=0; i<MAX_PROGRAMS; i++){
        if(args[i][0][0] != 0x0){
            notempty++;
        }
    }
    if(cnt != 0 && notempty != cnt + 1){
        return ERR_NOT_VALID_FORMAT;
    }
    return notempty;
}

bool isCommand(char* arg){
    if(strcmp(arg, "cd") || strcmp(arg, "ls") || strcmp(arg, "mkdir") || strcmp(arg, "cat") || strcmp(arg, "cp") || strcmp(arg, "mv")){
        return true;
    }
    return false;
}