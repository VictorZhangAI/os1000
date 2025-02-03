#ifndef __KERNEL_H__
#define __KERNEL_H__

struct sbiret
{
	long error;
	long value;
};

#define PANIC(fmt, ...) \
	do{ \
	printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
	for(;;); \
	}while(0) 

#endif
