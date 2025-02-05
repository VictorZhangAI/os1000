#include "fs.h"
#include "kernel.h"
#include "common.h"
#include "virtio.h"

struct file files[FILES_MAX];
uint8_t disk[DISK_MAX_SIZE];

int oct2int(char *oct, int len)
{
	int dec = 0;
	for(int i = 0; i < len; i++)
	{
		if(oct[i] < '0' || oct[i] > '7')
			break;

		dec = dec * 8 + (oct[i] - '0');
	}
	return dec;
}

void fs_init(void)
{
	for(uint32_t sector = 0; sector < sizeof(disk) / SECTOR_SIZE; sector++)
		read_write_disk(&disk[sector * SECTOR_SIZE], sector, false);

	uint32_t off = 0;
	for(int i = 0; i < FILES_MAX; i++)
	{
		struct tar_header *header = (struct tar_header *)&disk[off];
		if(header->name[0] == '\0')
			break;

		if(strcmp(header->magic, "ustar") != 0)
			PANIC("Invalid tar header: magic=\"%s\"", header->magic);

		int filesz = oct2int(header->size, sizeof(header->size));
		struct file *file = &files[i];
		file->in_use = true;
		strcpy(file->name, header->name);
		memcpy(file->data, header->data, filesz);
		file->size = filesz;
		printf("file: %s, size=%d\n", file->name, file->size);

		off += align_up(sizeof(struct tar_header) + filesz, SECTOR_SIZE);
	}
}
