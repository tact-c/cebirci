#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

int main(){
    char str[100];
    memset(str,'\0',100*sizeof(char));
    printf("please enter a mathematical expression:\n");
    fgets(str,100,stdin);
    expression* node=parse(str);
    expression* previous;
    do{
	    freetree(previous);
	    previous=node;
	    node=simplify(node);
    }while(!comparetree(node,previous));
    printf("the tree structure of the mathematical expression is as follows:\n");
    freetree(previous);
    printtree(node,2);
    freetree(node);
    return 0;
}
