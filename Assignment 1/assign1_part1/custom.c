#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char* argv[]){

  const char s[2] = " ";
  char str[35] = "Hello friends my name is FPSRussia";
  char* token;
  char* copy[35];
  for(int j=0;j<strlen(*copy); j++){
  	*copy[j]=str[j];
  	printf("copy[i]:%s\n",*copy[j]);
  }
  token = strtok(str,s);
  //printf("copy is: %s\n", copy);
  for(int i=0; i<3; i++){
 		while(token!=NULL){
  		printf("%s\n",token);
  		token = strtok(NULL,s);
  	}
  	token = strtok(*copy,s);
	}
  return 0;

}