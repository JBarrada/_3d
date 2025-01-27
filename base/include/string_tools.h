#ifndef STRING_TOOLS_H
#define STRING_TOOLS_H

#include "math.h"
#include <stdint.h>

bool is_whitespace(char);
bool is_newline(char);
bool is_numeric(char);
int next_non_whitespace(char*, int);
int next_line(char*, int);
int next_numeric(char*, int);
int next_char(char*, char, int);
double atod(char*);
int atoi(char*);

#endif