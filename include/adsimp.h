#ifndef ADSIMP_H
#define ADSIMP_H
#include <stdbool.h>
#include "parser.h"


expression* simplify(expression* node);
bool comparetree(expression *node, expression *previous);
#endif
