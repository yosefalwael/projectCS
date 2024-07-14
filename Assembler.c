#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

void mode(char *,int *);
char* clean(char *);
char** classing(char* );
char* opcodedecoder(char* );
char * regdecoder(char *);
char* f(char* ,int t,int );
int checkforpsudo(char* );
//defining

#define MEMORY_SIZE 4096
#define MAX_LINE_LEN 500
#define MAX_LABEL_LEN 50
int main(int argv, char* argc[]) {
	//if (argv != 3) {
		//printf("not valid args");
		//return 0;
	//}
	
	//printf("%s%s",argc[1],argc[2]);
	char** assembly, ** labels,**assembler;
	char* filename, * memin, *pre, * line = NULL;
	int* labels_ind;
	int i, line_num, LInd;
	ssize_t read;
	size_t len = 0;
	FILE* fptr,*wfile;
	
	//filename = argc[1];
	//memin = argc[2];
	
	filename = "input_1.txt";
	memin = "memin.txt";
	
	labels_ind = (int*)malloc(MEMORY_SIZE * sizeof(int));
	labels = (char**)malloc(MEMORY_SIZE * sizeof(char*));
	assembly = (char**)malloc(MEMORY_SIZE * sizeof(char*));
    assembler = (char**)malloc(MEMORY_SIZE * sizeof(char*));
	for (i = 0;i < MEMORY_SIZE;i++) {
		assembly[i] = (char*)malloc(MAX_LINE_LEN * sizeof(char));
        for (int g=0;g<5;g++){
            assembly[i][g]='0';
        }
        assembly[i][5]='\0';
		labels[i] = (char*)malloc(MAX_LABEL_LEN * sizeof(char));
        assembler[i] = (char*)calloc(6,sizeof(char));
        for (int g=0;g<5;g++){
            assembler[i][g]='0';
        }
        assembler[i][5] = '\0';

	}
	fptr = fopen(filename, "r");
    wfile = fopen(memin, "w");
	line_num = 0;
	LInd = 0;
    int jump;
	while ((read = getline(&line, &len, fptr)) != -1) {
		pre = strtok(line, "#");
        pre = clean(pre);
        if(strstr(pre,"imm")){
            jump = 2;
        }
        else{
            jump =1;
        }
        int *res = (int*)calloc(2,sizeof(int));
        mode(pre,res);
        int mode_ind  = res[0];
        int end_label =res[1];
        char label[50];
        char ass[500];
        switch(mode_ind){
            case 0 :                
                strcpy(assembly[line_num],pre);
                line_num+=jump;
                break;
            case 1:
                strcpy(ass,pre+end_label+1);
                strcpy(assembly[line_num],ass);
                strcpy(label,pre);
                label[end_label] = '\0';
                strcpy(labels[LInd],label);
                labels_ind[LInd++] = line_num;
                line_num+=jump;
                break;
            case 2:
                strcpy(label,pre);
                label[end_label] = '\0';
                strcpy(labels[LInd],label);
                labels_ind[LInd++] = line_num;
                break;
        }
        }
    char** res =NULL;
    char res2[6] ={'0'};
    res2[5] = '\0';
    char *x1,*x2,*x3,*x4,*x5;
    char imm[6] = {'0'};
    char imm2[6]={'0'};
    imm2[5] = '\0';
    imm[5] = '\0';
    int place;
    printf("%s\n",assembly[1000]);
    for (place=0;place<MEMORY_SIZE;place++){
        if(checkforpsudo(assembly[place])){
            char* start = assembly[place];
            while((*start<'0')||(*start>'9')){
                start++;
            }
            int last_ind = 0;
            int address_ishex =0;
            int val_ishex =0;
            char* address,*val;
            int final_address;
            if((*(start+1) == 'x')||(*(start+1) == 'X')){
                address_ishex =1;
                start = start+2;
                }

            while((*(start+last_ind)!=' ')&&(*(start+last_ind)!='\t')){
                    last_ind++;
                }
            address = f(start,0,last_ind-1);    
            start = start+last_ind;
            while((*start<'0')||(*start>'9')){
                start++;
            }
            if((*(start+1) == 'x')||(*(start+1) == 'X')){
                val_ishex = 1;
                start = start+2;
                }
            last_ind = 0;    
             while((*(start+last_ind)!=' ')&&(*(start+last_ind)!='\t')&&(*(start+last_ind)!='\n')){
                    last_ind++;
                }
             val = f(start,0,last_ind-1);
            
            if(address_ishex){
                sscanf(address,"%X",&final_address);
            } 
            else{
                final_address =atoi(address);
            }  
            if(!(val_ishex)){
                int a = atoi(val);
                sprintf(val,"%X",a);
            } 
            int lenv = strlen(val);
            char new_val[6];
            new_val[5] = '\0';
            for(int n =0;n<lenv;n++){
                new_val[5-lenv+n] = val[n];

            }
            for(int f =0;f<5-lenv;f++){
                new_val[f] = '0';
            }
            strcpy(assembler[final_address],new_val);
            char nn[] = "nnnnn";
            strcpy(assembly[final_address],nn);
            
            
            continue;
        }
        if(!(strcmp(assembly[place],"nnnnn"))){
            continue;
        }
        if(!(strcmp(assembly[place],"00000"))){
            strcpy(assembler[place],"00000\0");
        }
        else{
        res = classing(assembly[place]);
        x1 = opcodedecoder(res[0]);
        x2 = regdecoder(res[1]);
        x3 = regdecoder(res[2]);
        x4 = regdecoder(res[3]);
        res2[0] = x1[0];
        res2[1] = x1[1];
        res2[2] = x2[0];
        res2[3] = x3[0];
        res2[4] = x4[0];
        //fprintf(wfile,"%s\n",res2);
        strcpy(assembler[place],res2);
        if(*res[5] == '1'){
            if((res[4][0] == '0')&&(res[4][1] == 'x')){
                int off = (strlen(res[4])-2);
                for(i=0;i<off;i++){
                    imm[5-i] = res[4][i+2];
                }
            }
            else{
                if(('0'<=res[4][0])&&(res[4][0]<='9')){
                    int a = atoi(res[4]);
                    sprintf(imm2,"%x",a);
                    int off = strlen(imm2);
                    for(i=0;i<off;i++){
                    imm[5-off+i] = imm2[i];
                    }
                for (int i=0;i<5-off;i++){
                    imm[i] = '0';
                }
                }
                else{
                    if(res[4][0]=='-'){
                        int a = atoi(res[4]);
                        char immb[8];
                        sprintf(immb,"%x",a);
                        for (int c =0;c<5;c++){
                            imm[c] = immb[c+3];
                        }
                        imm[5]='\0';

                    }
                    else{
                        int ind = 0;
                        for (i = 0;i<MEMORY_SIZE;i++){
                            if(strcmp(res[4],labels[i])==0){
                                ind = i;
                                break;
                            }    
                        }
                        int v = labels_ind[ind];
                        sprintf(imm2,"%x",v);
                        int off = strlen(imm2);
                        for(i=0;i<off;i++){
                        imm[5-off+i] = imm2[i];
                       
                        }
                        for (int i=0;i<5-off;i++){
                            imm[i] = '0';
                        }
                    }

                }
            }
           strcpy(assembler[place+1],imm);
        place++;
         
        }
        }

        
        
    }    

    for(int iter =0;iter<MEMORY_SIZE;iter++){
        fprintf(wfile,"%s\n",assembler[iter]);
    }
	fclose(fptr);
    fclose(wfile);
	
}
/*
a function that take a line from the inputfile
and returns
0,0 if its doesnt contain a label
1,i there is a label and assemble line in the same line i in the ind of the ':'
2,i if there is only a label
*/
void mode(char *line,int *res){
    int label = 0;   
    for (int i=0;i<500;i++){
        if(line[i]==':'){
            res[1] = i;
            for (int j =i+1;j<500;j++){
                if((line[j] == '\n')){
                    break;
                }
                if(line[j]!=' '){
                    res[0] =1;
                    return;
                }
            }
            res[0] = 2;
            return;
        }
    }

}
/*
a function that take a line from the inputfile
and returns 
the line without spaces
*/
char* clean(char *line){
    char* start = line;
    while(*(start)==' '){
        start++;
    }
    return start;
}
char** classing(char* line){
    char** res = (char**)calloc(6,sizeof(char*));
    char* start =line;
    char *opcode,*rd,*rs,*rt,*conslab;
    int offset = 0;
    while((*(start)==' ')||(*(start) == '\t')){
        start++;
    }
    int slicing[10] ={0};
    int curr_ind =0;
    while(start[curr_ind]!=' '){
        curr_ind++;
    }
    slicing[1] = curr_ind-1;
    
    for(int b=1;b<5;b++){
        while((start[curr_ind]==' ')||(start[curr_ind]==',')){
        curr_ind++;
            }
        slicing[2*b] = curr_ind;
        if(b==4){
            break;
        }
        while((start[curr_ind]!=',')&&(start[curr_ind]!=' ')){
            curr_ind++;
            }
            slicing[2*b+1] = curr_ind-1;
    }
    
    while((start[curr_ind]!=' ')&&(start[curr_ind]!='\t')&&(start[curr_ind]!='\n')){
        curr_ind++;
    }
    slicing[9] = curr_ind-1;
    res[0] = f(start,slicing[0],slicing[1]);
    res[1] = f(start,slicing[2],slicing[3]);
    res[2] = f(start,slicing[4],slicing[5]);
    res[3] = f(start,slicing[6],slicing[7]);
    res[4] = f(start,slicing[8],slicing[9]);
    if (strstr(start,"$imm") != NULL){
        res[5] = "1";
    }
    else{
    res[5] = "0";
    }
    return res;

}
char* f(char* line,int start,int end){
    char* res = (char*)calloc(end-start+2,sizeof(char));
    int i;
    for(i = start;i<end+1;i++){
        res[i-start] = line[i];
    }
    res[i-start] = '\0';
    return res;
}
char* opcodedecoder(char* temp){
    if (!strcmp(temp, "add"))
		return "00";
	else if (!strcmp(temp, "sub"))
		return "01";
    else if (!strcmp(temp, "mul"))
		return"02" ;
	else if (!strcmp(temp, "and"))
		return "03";
	else if (!strcmp(temp, "or"))
		return "04";
    else if (!strcmp(temp, "xor"))
		return "05";
	else if (!strcmp(temp, "sll"))
		return "06";
	else if (!strcmp(temp, "sra"))
		return "07";
	else if (!strcmp(temp, "srl"))
		return "08";
	else if (!strcmp(temp, "beq"))
		return "09";
	else if (!strcmp(temp, "bne"))
		return "0a";
	else if (!strcmp(temp, "blt"))
		return "0b";
	else if (!strcmp(temp, "bgt"))
		return "0c";
	else if (!strcmp(temp, "ble"))
		return "0d";
	else if (!strcmp(temp, "bge"))
		return "0e";
	else if (!strcmp(temp, "jal"))
		return "0f";
	else if (!strcmp(temp, "lw"))
		return "10";
	else if (!strcmp(temp, "sw"))
		return "11";
	else if (!strcmp(temp, "reti"))
		return "12";
	else if (!strcmp(temp, "in"))
		return "13";
	else if (!strcmp(temp, "out"))
		return "14";
	else if (!strcmp(temp, "halt"))
		return "15";

	return "-1";
}
char * regdecoder(char *temp) {
	if (!strcmp(temp, "$zero"))
		return "0";
	else if (!strcmp(temp, "$imm"))
		return "1";
	else if (!strcmp(temp, "$v0"))
		return "2";
	else if (!strcmp(temp, "$a0"))
		return "3";
	else if (!strcmp(temp, "$a1"))
		return "4";
    else if (!strcmp(temp, "$a2"))
		return "5";
    else if (!strcmp(temp, "$a3"))
		return "6";        
	else if (!strcmp(temp, "$t0"))
		return "7";
	else if (!strcmp(temp, "$t1"))
		return "8";
	else if (!strcmp(temp, "$t2"))
		return "9";
	else if (!strcmp(temp, "$s0"))
		return "a";
	else if (!strcmp(temp, "$s1"))
		return "b";
	else if (!strcmp(temp, "$s2"))
		return "c";
	else if (!strcmp(temp, "$gp"))
		return "d";
	else if (!strcmp(temp, "$sp"))
		return "e";
	else if (!strcmp(temp, "$ra"))
		return "f";
	return "asfasg";
}
int checkforpsudo(char* line){
    char* start = line;
     while((*(start)==' ')||(*(start) == '\t')){
        start++;
    }
    if(*(start) == '.'){
        return 1;
    }
    return 0;
}
