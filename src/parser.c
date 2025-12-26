#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

int pos = 0;
char* input;

typedef enum{
    TK_NUM,
    TK_VAR,
    TK_PLUS,
    TK_MIN,
    TK_MULT,
    TK_DIV,
    TK_PTSO,
    TK_PTSC,
    TK_EQ,
    TK_EOF
} tokentype;

typedef struct{
    tokentype type;
    double val;
    char name;
} token;

typedef enum{
    ND_NUM,
    ND_VAR,
    ND_PLUS,
    ND_MIN,
    ND_MULT,
    ND_DIV,
    ND_EQ
} nodetype;

token currenttoken;

typedef struct expression{
    nodetype type;
    double value;
    char name;
    struct expression* left;
    struct expression* right;
} expression;

void advance(){
    while(isspace(input[pos])){
        pos++;
    }
    if(isalpha(input[pos])){
        currenttoken.type=TK_VAR;
        currenttoken.name=input[pos];
	pos++;
        return;
    }
    if(isdigit(input[pos])){
        currenttoken.type=TK_NUM;
        currenttoken.val=0;
        while(isdigit(input[pos])){
            currenttoken.val=(currenttoken.val*10)+(input[pos]-'0');
            pos++;
        }
        return;
    }
    switch (input[pos])
    {
    case '+':
    currenttoken.type=TK_PLUS;
        break;
    case '-':
    currenttoken.type=TK_MIN;
        break;
    case '*':
    currenttoken.type=TK_MULT;
        break;
    case '/':
    currenttoken.type=TK_DIV;
        break;
    case '(':
    currenttoken.type=TK_PTSO;
        break;
    case ')':
    currenttoken.type=TK_PTSC;
        break;
    case '=':
    currenttoken.type=TK_EQ;
        break;
    case '\0':
    currenttoken.type=TK_EOF;
        break;
    
    default:
    printf("No such character exists!\n");
        break;
    }
    pos++;
    return;
}

expression* makenumber(double val){
    expression* node=malloc(sizeof(expression));
    node->type=ND_NUM;
    node->value=val;
    node->left=NULL;
    node->right=NULL;
    return node;
}

expression* makevariable(char name){
    expression* node=malloc(sizeof(expression));
    node->type=ND_VAR;
    node->name=name;
    node->left=NULL;
    node->right=NULL;
    return node;
}

expression* makebinary(nodetype op, expression* left, expression* right){
    expression* node=malloc(sizeof(expression));
    node->type=op;
    node->left=left;
    node->right=right;
    return node;
}

expression* parsefactor();
expression* parseterm();
expression* parseexp();

expression* parsefactor(){
    if(currenttoken.type==TK_NUM){
        double val=currenttoken.val;
        advance();
        return makenumber(val);
    }
    if(currenttoken.type==TK_VAR){
        char name=currenttoken.name;
        advance();
        return makevariable(name);
    }
    if(currenttoken.type==TK_PTSO){
        advance();
        expression* expr=parseexp();
        advance();
        return expr;
    }
    if(currenttoken.type==TK_EOF) return NULL;

    return NULL;
}

expression* parseterm(){
    expression* left=parsefactor();
    while(currenttoken.type==TK_DIV||currenttoken.type==TK_MULT){
        tokentype op=currenttoken.type;
        advance();
        expression* right=parsefactor();
        if(op==TK_MULT){
            left=makebinary(ND_MULT,left,right);
        }
        else{
            left=makebinary(ND_DIV,left,right);
        }
    }
    return left;
}

expression* parseexp(){
    expression* left=parseterm();
    while (currenttoken.type==TK_PLUS||currenttoken.type==TK_MIN)
    {
        tokentype op = currenttoken.type;
        advance();
        expression* right=parseterm();
        if(op==TK_PLUS){
            left=makebinary(ND_PLUS,left,right);
        }
        else{
            left=makebinary(ND_MIN,left,right);
        }
    }
    return left;
    
}

expression* parseeq(){
    expression* left=parseexp();
    if(currenttoken.type==TK_EQ){
        advance();
        expression* right=parseexp();
        left=makebinary(ND_EQ,left,right);
    }
    return left;
}

expression* parse(char* string){
    input=string;
    pos=0;
    advance();
    return parseeq();
}

void printtree(expression* node, int indent){
    if(!node) return;

    for(int i=0; i<=indent; i++) printf(" ");

    switch (node->type)
    {
    case ND_NUM:
    printf("%.0f\n",node->value);
        break;
    case ND_VAR:
    printf("%c\n",node->name);
        break;
    case ND_PLUS:
    printf("PLUS:\n");
    printtree(node->left,indent+2);
    printtree(node->right,indent+2);
        break;
    case ND_MIN:
    printf("MIN:\n");
    printtree(node->left,indent+2);
    printtree(node->right,indent+2);
    break;
    case ND_MULT:
    printf("MULT:\n");
    printtree(node->left,indent+2);
    printtree(node->right,indent+2);
    break;
    case ND_DIV:
    printf("DIV:\n");
    printtree(node->left,indent+2);
    printtree(node->right,indent+2);
    break;
    case ND_EQ:
    printf("EQ:\n");
    printtree(node->left,indent+2);
    printtree(node->right,indent+2);
    break;
}
}

void freetree(expression* node){
    if(!node) return;
    freetree(node->left);
    freetree(node->right);
    free(node);
}



