#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include <stdbool.h>

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



