#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcre.h>

void command_parse(char* line);
void apc_print(char* line);

int main(int argc, char* argv[]){
  if (argc < 2){
    printf("FATAL: no file passed\n");
    return 0;
  }
  FILE* inFile = fopen(argv[1], "r");
  if(inFile == NULL){
    printf("FATAL: coulnd't open file %s\n", argv[1]);
    return 0;
  }
  const char* errorptr;
  int erroroffset;
  int ovector[30];

  const char* pattern = "print|var.+";
  
  pcre* re = pcre_compile(pattern,
			  0,
			  &errorptr,
 			  &erroroffset,
			  NULL);

 if(re==NULL){
   printf("FATAL: PCRE Compilation failed\n");
   return 0;
 }
  
  char buffer[2048];
  int line = 0;
  
  while(fgets(buffer, 2048, inFile)!=NULL){
    sscanf(buffer, "%[^\n]", buffer);
    int rc = pcre_exec(re,
		       NULL,
		       buffer,
		       strlen(buffer),
		       0,
		       0,
		       ovector,
		       30);
    if(rc < 0){
      printf("rc is: %d\n", rc);
      printf("error on line %d, unrecognised command \"%s\"\n", line, buffer);
    }else {
      command_parse(buffer);
    }
    line++;
  }
  return 0;
}

void command_parse(char* line){
  char* fullLine = strdup(line);
  char* brokenLine = strtok(line, " ");
  if(strcmp(brokenLine, "print")==0){
    apc_print(fullLine);
  }else {
    printf("match found: %s\n", line);
  }
}


void apc_print(char* line){ 
  int strstart = 0;
  line += 6;
  for (strstart; strstart < 2 && (char)*line!='\0'; line++){
    if((char)*line == '"'){
      strstart++;
    }else{
      if(strstart!=0){
	printf("%c", (char)*line);
      }
    }
  }
  printf("\n");
}
