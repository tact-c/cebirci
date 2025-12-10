#ifndef PARSER_H
#define PARSER_H
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

typedef struct expression{
    nodetype type;
    double value;
    char name;
    struct expression* left;
    struct expression* right;
} expression;

expression* makebinary(nodetype op, expression* left, expression* right);
expression* makenumber(double val);
expression* makevariable(char name);
expression* parse(char* string);
void printtree(expression* node, int indent);
void freetree(expression* node);
#endif
