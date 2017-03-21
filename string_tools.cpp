#include "string_tools.h"


bool is_whitespace(char c) {
	switch (c) {
		case ' ':
		case '\t':
		case '\n':
		case '\r':
		case '\f': 
		case 0: return true;
	}
	return false;
}

bool is_newline(char c) {
	switch (c) {
		case '\n':
		case '\r':
		case '\f': 
		case 0: return true;
	}
	return false;
}

bool is_numeric(char c) {
	switch (c) {
		case '-': 
		case '+': 
		case '.': 
		case '0': 
		case '1': 
		case '2': 
		case '3': 
		case '4': 
		case '5': 
		case '6': 
		case '7': 
		case '8': 
		case '9': return true;	
	}
	return false;
}

bool starts_with(char* data, char* match) {
	for (int i=0; i < sizeof(match); i++) {
		if (data[i] != match[i])
			return false;
	}
	return true;
}

int next_line(char* data, int i) {
	while (!is_newline(data[i])) {
		i++;
	}
	while (is_newline(data[i])) {
		i++;
	}
	return i;
}

int next_whitespace(char* data, int i) {
	while (is_whitespace(data[i])) {
		i++;
	}
	while (!is_whitespace(data[i])) {
		i++;
	}
	return i;
}

int next_non_whitespace(char* data, int i) {
	while (!is_whitespace(data[i])) {
		i++;
	}
	while (is_whitespace(data[i])) {
		i++;
	}
	return i;
}

int next_numeric(char* data, int i) {
	while (is_numeric(data[i])) {
		i++;
	}
	while (!is_numeric(data[i])) {
		i++;
	}
	return i;
}

int next_char(char* data, char c, int i) {
	//while (data[i] == c) {
		//i++;
	//}
	while (data[i] != c) {
		i++;
	}
	return i;
}

double atod(char* data) {
	int sign = 1;
	double value = 0;
	
	int decimal = -1;
	int i = 0;
	while (!is_whitespace(data[i]) && is_numeric(data[i])) {
		switch (data[i]) {
			case '-': sign = -1; break;
			case '+': sign = 1; break;
			case '.': decimal = i; break;
			
			case '0': value *= 10; value += 0; break;
			case '1': value *= 10; value += 1; break;
			case '2': value *= 10; value += 2; break;
			case '3': value *= 10; value += 3; break;
			case '4': value *= 10; value += 4; break;
			case '5': value *= 10; value += 5; break;
			case '6': value *= 10; value += 6; break;
			case '7': value *= 10; value += 7; break;
			case '8': value *= 10; value += 8; break;
			case '9': value *= 10; value += 9; break;			
		}
		i++;
	}
	
	value *= sign;
	
	if (decimal == -1) {
		return value;
	}
	return (value / pow(10, (i - decimal) - 1));
}

int atoi(char* data) {
	int sign = 1;
	int value = 0;
	
	int i = 0;
	while (!is_whitespace(data[i]) && is_numeric(data[i])) {
		switch (data[i]) {
			case '-': sign = -1; break;
			case '+': sign = 1; break;
			
			case '0': value *= 10; value += 0; break;
			case '1': value *= 10; value += 1; break;
			case '2': value *= 10; value += 2; break;
			case '3': value *= 10; value += 3; break;
			case '4': value *= 10; value += 4; break;
			case '5': value *= 10; value += 5; break;
			case '6': value *= 10; value += 6; break;
			case '7': value *= 10; value += 7; break;
			case '8': value *= 10; value += 8; break;
			case '9': value *= 10; value += 9; break;			
		}
		i++;
	}
	
	value *= sign;
	
	return value;
}