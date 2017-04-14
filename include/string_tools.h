#ifndef STRING_TOOLS_H
#define STRING_TOOLS_H

#include "math.h"
#include <stdint.h>

bool is_whitespace(char);
bool is_newline(char);
bool is_numeric(char);
bool is_hex(char);
bool starts_with(char* data, char* match);
int next_whitespace(char*, int);
int next_non_whitespace(char*, int);
int next_line(char*, int);
int next_numeric(char*, int);
int next_char(char*, char, int);
uint32_t atoi_hex(char* data);
double atod(char*);
int atoi(char*);

#endif