#include <elf.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <unistd.h>
#include <readline/chardefs.h>

#include "le.elf.h"

#define KEY_VALUE_DELIM ": "

void
read_elf_header (struct elf32_hdr *e_hdr, char *buff);

int main (int argc, char *argv[])
{
	if (argc == 1) {
		fprintf(stderr, "read_header [options] <objfile>\n");
		return 1;
	}

	struct elf32_hdr *e_hdr;

	int fd	= open(argv[1], O_RDWR);
	if (fd < 0) {
		perror("open:");
		return 1;
	}

	int rd = read(fd, (void *) e_hdr, 16);
	if (fd < 0) {
		perror("read:");
		return 1;
	}

	close(fd);

	char buff[100];
	
	read_elf_header(e_hdr, buff);

	printf("%s", buff);

	return 0;
}

static inline int
read_elf_magic (struct elf32_hdr *e_hdr, char *buff)
{
	char *values[] = {NULL};
	struct e_ident_el e_magic = 
	{
		"Magic(ELFMAG1-ELFMAG3)",
			KEY_VALUE_DELIM,
			NULL,
			values

	};

	int count;
//null byte issue, break:

	count = sprintf(buff, "%s%s", e_magic.name, e_magic.pad_start);

	count += sprintf(buff+count, "%c%c%c", e_hdr->e_ident[EI_MAG1],
			e_hdr->e_ident[EI_MAG2], e_hdr->e_ident[EI_MAG3]);

	buff[count++] = NEWLINE;
	return count;
}

static inline int
read_elf_class (struct elf32_hdr *e_hdr, char *buff)
{
	int count;
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
			.pad_end = "\n",
			.values = values 
	};

//to solve the null byte issue do:

	count = sprintf(buff, "%s", ei_class.name);

	// then do:

	count += sprintf(buff+count, "%s", ei_class.pad_start);

	switch (e_hdr->e_ident[EI_CLASS]) {
		case ELFCLASSNONE: 
			{
				count += sprintf(buff+count,
						"%s", ei_class.values[ELFCLASSNONE]); 
				break; 			      }
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
		default:
			{
				count += sprintf(buff+count, "Unknown Class");
				break;
			}
	}

	buff[count++] = NEWLINE;
	return count;
}

/*static inline int
read_elf_data (struct elf32_hdr *e_hdr, char *buff)
{

}*/

static inline int
read_elf_ident (struct elf32_hdr *e_hdr, char *buff)
{
	int count = 0;

	count += read_elf_magic(e_hdr, buff);
	count += read_elf_class(e_hdr, buff+count);
	return count;
}

void
read_elf_header (struct elf32_hdr *e_hdr, char *buff)
{
	buff += read_elf_ident(e_hdr, buff);
	*buff = 0;

}
