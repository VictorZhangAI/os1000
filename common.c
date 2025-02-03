#include "common.h"

extern void putchar(char ch);

void printf(const char *fmt, ...)
{
	va_list vargs;
	va_start(vargs, fmt);

	while(*fmt)
	{
		if(*fmt == '%')
		{
			fmt++;

		}	
	}
}
