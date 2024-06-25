#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>


//defining

#define MEMORY_SIZE 4096;
#define MAX_LINE_LEN 500;
#define MAX_LABEL_LEN 50;
int main(int argv, char* argc) {
	if (argv != 3) {
		printf("not valid args");
		return 0;
	}
	char** assembly, ** labels;
	char* filename, * memin, * line = NULL;
	int* labels_ind;
	int i, line_num;
	ssize_t read;
	size_t len = 0;
	FILE* fptr;
	filename = argc[1];
	memin = argc[2];
	labels_ind = (int*)malloc(MEMORY_SIZE * sizeof(int));
	labels = (char**)malloc(MEMORY_SIZE * sizeof(char*));
	assembly = (char**)malloc(MEMORY_SIZE * sizeof(char*));
	for (i = 0;i < MEMORY_SIZE;i++) {
		assembly[i] = (char*)malloc(MAX_LINE_LEN * sizeof(char));
		labels[i] = (char*)malloc(MAX_LABEL_LEN * sizeof(char));
	}
	fptr = fopen(filename, "r");
	line_num = 1;
	while ((read = getline(&line, &len, fptr)) != -1) {
		if (line[0] == "	") {
			
		}
	}
}

