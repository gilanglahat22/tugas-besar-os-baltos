#include "header/string.h"

void memcpy(byte *dest, byte *src, int n){
	int i;
	for(i=0; i<n; i++){
		dest[i] = src[i];
	}
}

int strlen(char *string){
	unsigned int len;
	len = 0;
	while(string[len] != '\0'){
		len++;
	}
	return len;
}

bool strcmp(char *s1, char *s2){
	bool isDiff;
	int i;
	int len;
	if (strlen(s1) != strlen(s2)) {
		return false;
	}
	isDiff = false;
	i = 0;
	len = strlen(s1);
	while(!isDiff && i<len){
		isDiff = s1[i]!=s2[i];
		i++;
	}
	return !isDiff;
}

void strcpy(char *dst, char *src){
	int i=0;
	while(src[i] != '\0'){
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

void clear(char *ptr, int n){
	int i = 0;
	for(i = 0; i < n; i++){
		ptr[i] = 0x0;   
	}
}

void strncpy(char *dst, char *src, int n){
	int i=0;
	while(src[i] != '\0' && i<n){
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

void strcat(char *target, char *dest, char *src){
	strcpy(target, dest);
	strcpy(target + strlen(target), src);
}

bool strncmp(char *s1, char *s2, int n){
	int i;
	if(strlen(s1) < n || strlen(s2) < n){
		return false;
	}
	for(i=0; i<n; i++){
		if(s1[i] != s2[i]){
			return false;
		}
	}
	return true;
}

void absPath(char *target, char *path){
	int i = 0, j, k, cur_idx = 0;
	char tmp[64], tmp_path[64];
	char new_path[64][64];


	// printf("%s\n", path);
	while(path[i] != 0x0){
		clear(tmp, 64);
		while(path[i] == '/') i++;
		j = 0;
		while(path[i] != '/' && path[i] != 0x0){
			tmp[j] = path[i];
			i++; j++;
		}
		tmp[j] = 0x0;
		// printf("%d\n", cur_idx);
		if(strcmp(tmp, "..")){
			if(cur_idx > 0){
				cur_idx --;
			}
		}
		else if(j != 0 && strcmp(tmp, ".") == 0){
			strcpy(new_path[cur_idx], tmp);
			cur_idx++;
		}
		if(path[i]!=0x0) i++;
	}
	k = 0;
	for(i=0; i<cur_idx; i++){
		// printf("%s\n", new_path[i]);
		j = 0;
		while(new_path[i][j] != 0x0){
			target[k] = new_path[i][j];
			j++; k++;
		}
		if(i < cur_idx -1){
			target[k] = '/';
			k++;
		}
	}
	target[k] = 0x0;
}

void parsePath(char *targetName, char *dirPath, char *srcPath){
	int i, j, len;
	len = strlen(srcPath);
	for(i = len-1; i>=0; i--){
		if(srcPath[i] == '/'){
			break;
		}
	}
	for(j=0; j<i; j++){
		dirPath[j] = srcPath[j];
	}
	if(i<0){
		dirPath[0] = 0x0;
	}
	else{
		dirPath[i] = 0x0;
	}
	for(j=i+1; j<len; j++){
		targetName[j-i-1] = srcPath[j];
	}
	targetName[len-i-1] = 0x0;
}