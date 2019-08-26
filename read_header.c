#include <elf.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/chardefs.h>

#include "le.elf.h"



void
read_elf_header (struct elf32_hdr *e_hdr, char *buff);

extern int
read_elf_osabi (struct elf32_hdr *e_hdr, char *buff);


int
decode_elf_value (int endianness, int *value);

int main (int argc, char *argv[])
{
	if (argc == 1) {
		fprintf(stderr, "read_header [options] <objfile>\n");
		return 1;
	}

	struct elf32_hdr *e_hdr = malloc(sizeof(struct elf32_hdr));

	if (e_hdr == NULL) {
		perror("malloc");
		return 1;
	}

	int fd	= open(argv[1], O_RDWR);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	int rd = read(fd, (void *) e_hdr, sizeof(*e_hdr));
	if (rd < 0) {
		perror("read");
		return 1;
	}

	close(fd);

	char buff[1000];
	
	read_elf_header(e_hdr, buff);

	printf("%s", buff);

	free(e_hdr);

	return 0;
}

static inline int
read_elf_e_type (struct elf32_hdr *e_hdr, char *buff)
{

#define ET_OS 5
#define ET_PROC 6

	int count = 0;
	char *values[] = 
	{
		[ET_NONE] = "No file type", 

		[ET_REL] = "Relocatable file", 

		[ET_EXEC] = "Executable file",

		[ET_DYN] = "Shared object file",

		[ET_CORE] = "Core file",

		[ET_OS] = "Operating system-specific(pending)",

		[ET_PROC] = "Processor-specific(pending)",

	};

	elf32_hdr_mem e_type = 
	{
		.name = "Object file type(e_type)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = values 
	};


	count += sprintf(buff, "%s%s", e_type.name, e_type.pad_start);

	unsigned short tmp = (unsigned short) decode_elf_value((int)
			e_hdr->e_ident[EI_DATA], (int *)&e_hdr->e_type) ;

	if (tmp >= ET_NONE || tmp <= ET_CORE) {

		count += sprintf(buff+count, "%s",
				e_type.values[tmp]);

	} else if (tmp >= ET_LOPROC || tmp <= ET_HIPROC) {

		count += sprintf(buff+count, "%s",
				e_type.values[ET_PROC]);

	} else if (tmp >= ET_LOOS || tmp <= ET_HIOS) {

		count += sprintf(buff+count, "%s",
				e_type.values[ET_OS]);

	}


	buff[count++] = NEWLINE;
	return count;
}

static inline int
read_elf_magic (struct elf32_hdr *e_hdr, char *buff)
{
	char *values[] = {NULL};

	int count = 0;

	struct e_ident_el e_magic = 
	{
		"Magic(ELFMAG1-ELFMAG3)",
			KEY_VALUE_DELIM,
			NULL,
			values

	};

//to look good break sprintf() call into:

	count += sprintf(buff, "%s%s", e_magic.name, e_magic.pad_start);

	//and:

	count += sprintf(buff+count, "%c%c%c", e_hdr->e_ident[EI_MAG1],
			e_hdr->e_ident[EI_MAG2], e_hdr->e_ident[EI_MAG3]);

	buff[count++] = NEWLINE;
	return count;
}

static inline int
read_elf_class (struct elf32_hdr *e_hdr, char *buff)
{
	int count = 0;
	char *values[] = 
	{
		"Invalid Class", 
		"32-bits Objects", 
		"64-bits Objects"
	};

	struct e_ident_el ei_class = 
	{
		.name = "Class(EI_CLASS)",
		.pad_start = KEY_VALUE_DELIM,
		.pad_end = NULL,
		.values = values 
	};


	count += sprintf(buff, "%s%s", ei_class.name, ei_class.pad_start);


	switch (e_hdr->e_ident[EI_CLASS]) {

		case ELFCLASS32:
			{
				count += sprintf(buff+count, "%s",
						ei_class.values[ELFCLASS32]);
				break;
			}
		case ELFCLASS64:
			{
				count += sprintf(buff+count, "%s",
						ei_class.values[ELFCLASS64]);
				break;
			}
		case ELFCLASSNONE: default:
			{
				count += sprintf(buff+count,
						"%s", ei_class.values[ELFCLASSNONE]); 
				break; 			      }

	}

	buff[count++] = NEWLINE;
	return count;
}

static inline int
read_elf_data (struct elf32_hdr *e_hdr, char *buff)
{
	int count = 0;
	char *values[] = 
	{
		"Invalid Data Encoding",
		"Least Significant Byte",
		"Most Significant Byte"
	};

	struct e_ident_el ei_data = 
	{
		.name = "Data Encoding(EI_DATA)",
		.pad_start = KEY_VALUE_DELIM "2's Complement ",
		.pad_end = NULL,

		.values = values
	};

	count += sprintf(buff, "%s%s", ei_data.name, ei_data.pad_start); 

	switch (e_hdr->e_ident[EI_DATA]) {

		case ELFDATA2LSB:
			{

				count += sprintf(buff+count, "%s", ei_data.values[ELFDATA2LSB]); 
				break;

			}

		case ELFDATA2MSB:
			{

				count += sprintf(buff+count, "%s", ei_data.values[ELFDATA2MSB]); 
				break;

			}

		case ELFDATANONE: default:
			{

				count += sprintf(buff+count, "%s", ei_data.values[ELFDATANONE]); 
				break;

			}

	}

	buff[count++] = NEWLINE;
	return count;
}

static inline int
read_elf_version (struct elf32_hdr *e_hdr, char *buff)
{
	int count = 0;
	char *values[] = 
	{
		"Invalid",

		"Current"
	};

	struct e_ident_el ei_version = 
	{
		.name = "Version(EI_VERSION)",
		.pad_start = KEY_VALUE_DELIM ,
		.pad_end = " Version\n",

		.values = values
	};

	count += sprintf(buff, "%s%s", ei_version.name, ei_version.pad_start); 

	switch (e_hdr->e_ident[EI_VERSION]) {

		case EV_CURRENT:
			{

				count += sprintf(buff+count, "%s", ei_version.values[EV_CURRENT]); 
				break;

			}


		case EV_NONE: default:
			{

				count += sprintf(buff+count, "%s", ei_version.values[EV_NONE]); 
				break;

			}

	}

	count += sprintf(buff+count, "%s", ei_version.pad_end);

	return count;
}



static inline int
read_elf_ident (struct elf32_hdr *e_hdr, char *buff)
{
	int count = 0;

	count += read_elf_magic(e_hdr, buff);
	count += read_elf_class(e_hdr, buff+count);
	count += read_elf_data(e_hdr, buff+count);
	count += read_elf_version(e_hdr, buff+count);
	count += read_elf_osabi(e_hdr, buff+count);
	return count;
}

void
read_elf_header (struct elf32_hdr *e_hdr, char *buff)
{
	buff += read_elf_ident(e_hdr, buff);

	buff += read_elf_e_type(e_hdr, buff);

	*buff = 0;

}
