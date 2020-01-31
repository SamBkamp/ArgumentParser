#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcre.h>

void command_parse(char* line);

int main(int argc, char* argv[]){
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
      char* brokenLine = strtok(buffer, " ");
      command_parse(brokenLine);
    }
    line++;
  }
  return 0;
}

void command_parse(char* line){
  if(strcmp(line, "print")==0){
    line = strtok(NULL, " ");
    int line_length = strlen(line); 
    char buffer[100];
    for (line; line!=NULL && (char)line+(5-1) != '"'; line=strtok(NULL, " ")){
      printf("%s ", line);
    }
    printf("\n");
  }else {
    printf("match found: %s\n", line);
  }
}
