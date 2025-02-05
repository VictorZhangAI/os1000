#ifndef __USER_H__
#define __USER_H__

#include"common.h"

__attribute__((noreturn)) void exit(void);
void putchar(char ch);
int getchar(void);
int read(const char *filename, char *buf, int len);
int write(const char *filename, const char *buf, int len);

#endif
