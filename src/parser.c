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
    while(input[pos]==' '||input[pos]=='\b'||input[pos]=='\n'){
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
    printf("BÖYLE BİR KARAKTER YOK!\n");
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


expression* simplify(expression* node){
	if(!node) return NULL;
	expression* left = NULL;
	expression* right = NULL;
	if(node->left) left=simplify(node->left);
	if(node->right) right=simplify(node->right);
	
	switch(node->type){
		case ND_NUM: return makenumber(node->value);
			     break;
		case ND_VAR: return makevariable(node->name);
			     break;
		case ND_MULT:
			     if((left-> type == ND_NUM && left->value == 0) || (right->type == ND_NUM && right->value == 0)){
				     freetree(left);
				     freetree(right);
				     return makenumber(0);}
			     else if(left->type==ND_NUM && right->type==ND_NUM){
				     double result = left->value * right->value;
				     freetree(left);
				     freetree(right);
				     return makenumber(result);}
			     else if(left->type == ND_NUM && left->value == 1){
				     freetree(left);
				     return right;}
			     else if(right->type == ND_NUM && right->value == 1){
				     freetree(right);
				     return left;}
			     else if(left->type == ND_VAR || right->type == ND_VAR){
				     return makebinary(ND_MULT, left, right);}
			     break;
		case ND_PLUS:
			     if(left->type == ND_NUM && left->value == 0){
				     freetree(left);
				     return right;}
			     else if(right->type == ND_NUM && right->value == 0){
				     freetree(right);
				     return left;}
			     else if(left->type == ND_NUM && right->type == ND_NUM){
				     double result = left->value + right->value;
				     freetree(left);
				     freetree(right);
				     return makenumber(result);}
			     else return makebinary(ND_PLUS, left, right);
			     break;
		case ND_MIN:
			     if(right->type == ND_NUM && right->value == 0){
				     freetree(right);
				     return left;}
			     else if(left->type == ND_NUM && right->type == ND_NUM){
				     double result = left->value - right->value;
				     freetree(left);
				     freetree(right);
				     return makenumber(result);}
			     else if(left->type == ND_VAR || right->type == ND_VAR){
				     return makebinary(ND_MIN, left, right);}
			     break;
		case ND_DIV:
			     if(right->type == ND_NUM && right->value == 1){
				     freetree(right);
				     return left;}
			     else if(left->type == ND_NUM && left->value == 0){
				     freetree(left);
				     freetree(right);
				     return makenumber(0);}
			     else if(left->type == ND_NUM && right->type == ND_NUM){
				     double result = left->value / right->value;
				     freetree(left);
				     freetree(right);
				     return makenumber(result);}
			     else{
				     return makebinary(ND_DIV, left, right);}
			     break;
		case ND_EQ:
			     if(left==NULL && right!=NULL) return makebinary(ND_EQ, makenumber(0), right);
			     else if(right==NULL && left!=NULL) return makebinary(ND_EQ, left, makenumber(0));
			     else return makebinary(ND_EQ, left, right);
			     break;
		default: return NULL;
			 break;
	}
}

bool comparetree(expression* node, expression* previous){
	if(!node && !previous) return 1;
	if(!node || !previous) return 0;
	if(node->type != previous->type) return 0;
	switch(node->type){
		case ND_NUM:
			if(node->value != previous->value) return 0;
			break;
		case ND_VAR:
			if(node->name != previous->name) return 0;
			break;
		default:
			break;
	}
	return comparetree(node->left, previous->left)&&comparetree(node->right, previous->right);}
