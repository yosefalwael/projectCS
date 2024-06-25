#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>


//defining

#define MEMORY_SIZE 4096
#define MAX_LINE_LEN 500
#define MAX_LABEL_LEN 50
int main(int argv, char* argc[]) {
	printf("%d",argv);
	
	if (argv != 3) {
		printf("not valid args");
		return 0;
	}
	
	printf("%s%s",argc[1],argc[2]);
	char** assembly, ** labels;
	char* filename, * memin, *pre, * line = NULL;
	int* labels_ind;
	int i, line_num, LInd;
	ssize_t read;
	size_t len = 0;
	FILE* fptr;
	
	filename = argc[1];
	memin = argc[2];
	
	//filename = "input_1.txt";
	//memin = "memory.txt";
	
	labels_ind = (int*)malloc(MEMORY_SIZE * sizeof(int));
	labels = (char**)malloc(MEMORY_SIZE * sizeof(char*));
	assembly = (char**)malloc(MEMORY_SIZE * sizeof(char*));
	for (i = 0;i < MEMORY_SIZE;i++) {
		assembly[i] = (char*)malloc(MAX_LINE_LEN * sizeof(char));
		labels[i] = (char*)malloc(MAX_LABEL_LEN * sizeof(char));
	}
	fptr = fopen(filename, "r");
	line_num = 0;
	LInd = 0;
	while ((read = getline(&line, &len, fptr)) != -1) {
		if (line[0] == '	') {
			pre = strtok(line, "#");
			char* start = pre;
			strcpy(assembly[line_num++],pre);
		}
		else {
			pre = strtok(line, ":");
			strcpy(labels[LInd],pre);
			labels_ind[LInd++] = line_num;
		}
	}
	fclose(fptr);
	for (i = 0;i < 100;i++) {
		printf("%s\n", assembly[i]);
	}
}
