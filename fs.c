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

void fs_flush(void)
{
	memset(disk, 0, sizeof(disk));
	uint32_t off = 0;
	for(int file_i = 0; file_i < FILES_MAX; file_i++)
	{
		struct file *file = &files[file_i];
		if(!file->in_use)
			continue;

		struct tar_header *header = (struct tar_header *)&disk[off];
		memset(header, 0, sizeof(*header));
		strcpy(header->name, file->name);
		strcpy(header->mode, "000644");
		strcpy(header->magic, "ustar");
		strcpy(header->version, "00");
		header->type = '0';

		int filesz = file->size;
		for(int i = sizeof(header->size); i > 0; i--)
		{
			header->size[i - 1] = (filesz % 8) + '0';
			filesz /= 8;
		}

		int checksum = ' ' * sizeof(header->checksum);
		for(uint32_t i = 0; i < sizeof(struct tar_header); i++)
			checksum += (uint8_t)disk[off + i];

		for(int i = 5; i >= 0; i--)
		{
			header->checksum[i] = (checksum % 8) + '0';
			checksum /= 8;
		}

		memcpy(header->data, file->data, file->size);
		off += align_up(sizeof(struct tar_header) + file->size, SECTOR_SIZE);
	}

	for(uint32_t sector = 0; sector < sizeof(disk) / SECTOR_SIZE; sector++)
		read_write_disk(&disk[sector * SECTOR_SIZE], sector, true);

	printf("wrote %d bytes to disk\n", sizeof(disk));
}

struct file *fs_lookup(const char *filename)
{
	for(int i = 0; i < FILES_MAX; i++)
	{
		struct file *file = &files[i];
		if(!strcmp(file->name, filename))
			return file;
	}
	return NULL;
}